#include <ESP8266WiFi.h>      // Управление Wi-Fi модулем
#include <PubSubClient.h>     // Клиент MQTT протокола
#include <ArduinoJson.h>      // Генерация JSON для телеметрии
#include <OneWire.h>          // Протокол 1-Wire для DS18B20
#include <DallasTemperature.h> // Библиотека датчика пола
#include <Adafruit_AM2320.h>  // Датчик воздуха (температура/влажность)
#include <Wire.h>             // Шина I2C

// =================================================================================
// 1. КОНФИГУРАЦИЯ И ПИНЫ
// =================================================================================

// Учетные данные Wi-Fi.
// Реальные значения задаются в include/Secrets.h (файл в .gitignore) и в репозиторий не попадают.
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Авторизация MQTT (IP брокера определится автоматически как Gateway)
const char* mqtt_user = "YOUR_MQTT_USER";
const char* mqtt_pass = "YOUR_MQTT_PASSWORD";

// Пины Wemos D1 Mini
#define PIN_SCL D1           // I2C SCL (датчик AM2320)
#define PIN_SDA D2           // I2C SDA (датчик AM2320)
#define PIN_DS18B20 D5       // 1-Wire данные (датчик пола)
#define PIN_PIR D6           // Вход с ИК-датчика движения
#define PIN_PWM_LED D7       // ШИМ на драйвер TC4420 (лента)
#define PIN_SSR_FLOOR D8     // Выход на реле теплого пола

// Константы времени (миллисекунды)
const unsigned long LIGHT_PIR_TIMEOUT = 60000;       // 1 минута ( MD = ON )
const unsigned long LIGHT_SAFETY_TIMEOUT = 3600000;  // 1 час ( MD = OFF )
const unsigned long FLOOR_TIMEOUT_MD_ON = 3600000;   // 1 час ( MD = ON )
const unsigned long FLOOR_TIMEOUT_MD_OFF = 43200000; // 12 часов ( MD = OFF )
const unsigned long BOOT_PIR_DELAY = 10000;          // Игнорируем PIR первые 10 сек после старта
const unsigned long FADE_STEP_TIME = 20;             // Шаг розжига (итого 2 сек на 100 шагов)
const unsigned long SENSOR_INTERVAL = 15000;         // Опрос датчиков раз в 15 секунд

// =================================================================================
// 2. ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
// =================================================================================

// --- Параметры Света ---
float currentBrightness = 0.0;     // Текущий расчетный уровень яркости 0-100%
float targetBrightness = 0.0;      // Целевая яркость, куда плавно стремится ШИМ
int userBrightnessPercent = 100;    // Уровень яркости, заданный пользователем
bool lightState = false;           // Логическое состояние ВКЛ/ВЫКЛ
bool lightMoveDetection = false;    // Активация авто-света по движению

// --- Параметры Пола ---
float floorTemp = 0, airTemp = 0, airHum = 0; // Данные с сенсоров
float targetFloorTemp = 30.0;      // Уставка пола по умолчанию
bool floorMoveDetection = false;    // Активация авто-пола по движению
bool floorMasterState = false;      // Глобальный выключатель пола (кнопка в MQTT)
bool floorRelayActual = false;      // Реальное состояние реле (греет/не греет)

// --- РАЗДЕЛЬНЫЕ ТАЙМЕРЫ ДВИЖЕНИЯ ---
// lastLightMoveTime сбрасывается при движении PIR и командах СВЕТА
// lastFloorMoveTime сбрасывается при движении PIR и командах ПОЛА
unsigned long lastLightMoveTime = 0; 
unsigned long lastFloorMoveTime = 0; 

// Системные таймеры
unsigned long lastSensorRead = 0;    // Время последнего опроса датчиков
unsigned long lastTelemetry = 0;     // Время последней отправки JSON
unsigned long lastFadeTime = 0;       // Время последнего шага изменения яркости
unsigned long connectionLostTime = 0; // Для работы Watchdog
unsigned long bootTime = 0;           // Время подачи питания на ESP

// Флаги системы
bool connectionLost = false;      // Флаг потери связи с WiFi/MQTT
bool firstConnectDone = false;    // Флаг первой публикации состояний при старте
bool lastPirState = false;        // Хранение предыдущего состояния PIR

// Объекты периферии
WiFiClient espClient;
PubSubClient client(espClient);
OneWire oneWire(PIN_DS18B20);
DallasTemperature sensors(&oneWire);
Adafruit_AM2320 am2320 = Adafruit_AM2320();

// =================================================================================
// 3. ФУНКЦИИ ЛОГИКИ
// =================================================================================

/**
 * Расчет кривой CIE1931.
 * Превращает линейные проценты (0-100) в логарифмический ШИМ (0-1023),
 * чтобы человеческий глаз видел розжиг идеально плавным.
 */
int cie1931(float L) {
    if (L <= 0) return 0;
    if (L >= 100) return 1023;
    float out = (L <= 8) ? (L / 902.3) : pow((L + 16.0) / 116.0, 3.0);
    return (int)(out * 1023.0);
}

/**
 * Публикация физического состояния нагрева (состояние реле).
 * Вызывается только при фактическом изменении работы нагревателя.
 */
void publishFloorHeatState(bool state) {
    client.publish("home/hall/floor/floor_heat_state", state ? "ON" : "OFF", true);
    Serial.printf("[FLOOR] Relay Physical state -> %s\n", state ? "ON" : "OFF");
}

/**
 * Главная функция управления светом.
 * Мгновенно публикует статус light_state в MQTT.
 */
void setLight(bool on, String source) {
    if (lightState != on) {
        lightState = on;
        // Мгновенное подтверждение команды в MQTT
        client.publish("home/hall/light/light_state", on ? "ON" : "OFF", true);
        Serial.printf("[LIGHT] Internal state -> %s (Source: %s)\n", on ? "ON" : "OFF", source.c_str());
    }

    if (!on) {
        // Сбрасываем яркость в 100% только при выключении.
        // Это не мешает ползунку яркости HA работать во время включения.
        userBrightnessPercent = 100;
        targetBrightness = 0.0;
    } else {
        // Целевая яркость - то, что прислал HA или то, что было сохранено.
        targetBrightness = (float)userBrightnessPercent;
    }
}

/**
 * Логика пола (Термостат + Таймеры).
 * Опрос датчика в этой функции теперь блокирующий (надежный).
 */
void updateFloorLogic() {
    // 1. Блокирующий опрос температуры пола
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    // Проверка корректности (убираем ошибки шины -127 и 85)
    if (temp != -127.0 && temp != 85.0 && temp != 0) floorTemp = temp;

    // 2. Рассчитываем окно активности по времени
    unsigned long floorLimit = floorMoveDetection ? FLOOR_TIMEOUT_MD_ON : FLOOR_TIMEOUT_MD_OFF;
    unsigned long now = millis();
    unsigned long diff = (now > lastFloorMoveTime) ? (now - lastFloorMoveTime) : 0;
    bool floorWindowActive = (diff < floorLimit);
    bool oldRelay = floorRelayActual;

    // 3. Условие работы нагревателя (Мастер ВКЛ + Таймер не истёк)
    if (floorMasterState && floorWindowActive) {
        // Гистерезис 0.5 градуса от уставки
        if (floorTemp < (targetFloorTemp - 0.5)) floorRelayActual = true;
        else if (floorTemp > (targetFloorTemp + 0.5)) floorRelayActual = false;
    } else {
        floorRelayActual = false;
        // Если выключились по таймауту, извещаем MQTT
        if (!floorWindowActive && floorMasterState) {
            floorMasterState = false;
            client.publish("home/hall/floor/floor_state", "OFF", true);
            Serial.println("[FLOOR] Inactivity Timeout reached.");
        }
    }

    // 4. Физическое переключение реле SSR
    if (floorRelayActual != oldRelay) {
        digitalWrite(PIN_SSR_FLOOR, floorRelayActual);
        publishFloorHeatState(floorRelayActual);
    }
}

/**
 * Публикация всех "нулевых" состояний при первой загрузке контроллера.
 */
void publishInitialStates() {
    Serial.println("[SYS] First boot: Initializing all states to OFF in MQTT.");
    client.publish("home/hall/light/light_state", "OFF", true);
    client.publish("home/hall/light/brightness_state", "100", true);
    client.publish("home/hall/light/moveDetection_state", "OFF", true);
    client.publish("home/hall/floor/floor_state", "OFF", true);
    client.publish("home/hall/floor/floor_heat_state", "OFF", true);
    client.publish("home/hall/floor/moveDetection_state", "OFF", true);
    client.publish("home/hall/floor/targetTemp", String(targetFloorTemp).c_str(), true);
    client.publish("home/hall/light/move", "OFF", false);
}

// =================================================================================
// 4. MQTT CALLBACK (Прием команд)
// =================================================================================
void callback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (unsigned int i = 0; i < length; i++) message += (char)payload[i];
    String strTopic = String(topic);

    Serial.printf("[MQTT RECV] [%s] : %s\n", topic, message.c_str());

    // Группа команд Света
    if (strTopic == "home/hall/light/light") {
        lastLightMoveTime = millis(); // Сброс таймера света командой
        if (message == "ON") setLight(true, "MQTT");
        else setLight(false, "MQTT");
    } 
    else if (strTopic == "home/hall/light/brightness") {
        userBrightnessPercent = constrain(message.toInt(), 1, 100);
        if (lightState) targetBrightness = (float)userBrightnessPercent;
    }
    else if (strTopic == "home/hall/light/moveDetection") {
        lastLightMoveTime = millis(); // Сброс таймера света при смене режима
        lightMoveDetection = (message == "ON");
        client.publish("home/hall/light/moveDetection_state", message.c_str(), true);
    }
    // Группа команд Пола
    else if (strTopic == "home/hall/floor/floor") {
        if (message == "ON") lastFloorMoveTime = millis(); // Сброс таймера пола командой
        floorMasterState = (message == "ON");
        client.publish("home/hall/floor/floor_state", message.c_str(), true);
        updateFloorLogic(); // Мгновенный пересчет при команде
    }
    else if (strTopic == "home/hall/floor/moveDetection") {
        lastFloorMoveTime = millis();
        floorMoveDetection = (message == "ON");
        client.publish("home/hall/floor/moveDetection_state", message.c_str(), true);
        updateFloorLogic();
    }
    else if (strTopic == "home/hall/floor/targetTemp") {
        targetFloorTemp = message.toFloat();
        Serial.printf("[FLOOR] Target Temp -> %.1f\n", targetFloorTemp);
        updateFloorLogic();
    }
}

/**
 * Переподключение к MQTT (авто-IP через Gateway).
 */
void reconnect() {
    if (WiFi.status() == WL_CONNECTED && !client.connected()) {
        IPAddress gateway = WiFi.gatewayIP(); 
        client.setServer(gateway, 1883);
        
        Serial.printf("[NET] Connecting MQTT at Gateway: %s... ", gateway.toString().c_str());
        
        String clientId = "ESP8266_Hall_VFinal_" + String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
            Serial.println("CONNECTED");
            if (!firstConnectDone) { publishInitialStates(); firstConnectDone = true; }

            // Подписка на топики
            client.subscribe("home/hall/light/light");
            client.subscribe("home/hall/light/brightness");
            client.subscribe("home/hall/light/moveDetection");
            client.subscribe("home/hall/floor/floor");
            client.subscribe("home/hall/floor/moveDetection");
            client.subscribe("home/hall/floor/targetTemp");
            connectionLost = false;
        } else {
            Serial.printf("FAILED (rc=%d)\n", client.state());
        }
    }
}

// =================================================================================
// 5. SETUP
// =================================================================================
void setup() {
    Serial.begin(115200);
    bootTime = millis();

    // Настройка ШИМ 4кГц для TC4420
    analogWriteFreq(4000);   
    analogWriteRange(1023);  
    pinMode(PIN_PWM_LED, OUTPUT);
    analogWrite(PIN_PWM_LED, 0); 
    
    // Пины реле и датчика движения
    pinMode(PIN_SSR_FLOOR, OUTPUT);
    digitalWrite(PIN_SSR_FLOOR, LOW); 
    pinMode(PIN_PIR, INPUT);

    // WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
    Serial.println("\n[NET] WiFi Connected.");

    client.setCallback(callback);
    client.setBufferSize(512);

    // Инициализация датчиков
    Wire.begin(PIN_SDA, PIN_SCL);
    am2320.begin();
    sensors.begin();
    
    // Начальные метки времени таймеров
    lastLightMoveTime = millis();
    lastFloorMoveTime = millis();
    
    Serial.println("[SYS] Hall System Ready.");
}

// =================================================================================
// 6. LOOP
// =================================================================================
void loop() {
    unsigned long now = millis();

    // WiFi/MQTT Watchdog
    if (WiFi.status() != WL_CONNECTED || !client.connected()) {
        if (!connectionLost) { connectionLostTime = now; connectionLost = true; }
        if (now - connectionLostTime > 300000) ESP.restart();
        reconnect();
    }
    client.loop();

    // --- 6.1. ОБРАБОТКА PIR ---
    bool pirActive = digitalRead(PIN_PIR);
    if (pirActive != lastPirState) {
        if (now - bootTime > BOOT_PIR_DELAY) {
            if (pirActive) {
                Serial.println("[PIR] Movement Detected: Refreshing Timers.");
                client.publish("home/hall/light/move", "ON");
                
                // Физическое движение ОБНУЛЯЕТ ОБА раздельных таймера
                lastLightMoveTime = millis();
                lastFloorMoveTime = millis();
                
                if (lightMoveDetection && !lightState) setLight(true, "PIR");
                if (floorMoveDetection && !floorMasterState) {
                    floorMasterState = true;
                    client.publish("home/hall/floor/floor_state", "ON", true);
                    updateFloorLogic();
                }
            } else {
                Serial.println("[PIR] Movement Stopped.");
                client.publish("home/hall/light/move", "OFF");
            }
        }
        lastPirState = pirActive;
    }

    // --- 6.2. ТАЙМАУТ СВЕТА ---
    unsigned long lightLimit = lightMoveDetection ? LIGHT_PIR_TIMEOUT : LIGHT_SAFETY_TIMEOUT;
    unsigned long currentNow = millis();
    unsigned long lightDiff = (currentNow > lastLightMoveTime) ? (currentNow - lastLightMoveTime) : 0;

    if (lightState && (lightDiff > lightLimit)) {
        setLight(false, "AUTO_OFF");
    }

    // --- 6.3. ПЛАВНЫЙ РОЗЖИГ (2 сек) ---
    if (currentNow - lastFadeTime > FADE_STEP_TIME) {
        if (currentBrightness != targetBrightness) {
            float step = 1.0; 
            if (currentBrightness < targetBrightness) currentBrightness = min(currentBrightness + step, targetBrightness);
            else currentBrightness = max(currentBrightness - step, targetBrightness);
            
            analogWrite(PIN_PWM_LED, cie1931(currentBrightness));
            
            // Статус яркости шлем только по завершении перехода
            if (currentBrightness == targetBrightness) {
                client.publish("home/hall/light/brightness_state", String(userBrightnessPercent).c_str());
            }
        }
        lastFadeTime = currentNow;
    }

    // --- 6.4. ДАТЧИКИ И ПОЛ (Раз в 15 сек) ---
    if (currentNow - lastSensorRead > SENSOR_INTERVAL) {
        lastSensorRead = currentNow;
        
        // Чтение AM2320 (воздух)
        airTemp = am2320.readTemperature();
        airHum = am2320.readHumidity();
        
        // Чтение DS18B20 и логика термостата (пол)
        updateFloorLogic();
    }

    // --- 6.5. ТЕЛЕМЕТРИЯ JSON (Раз в 10 сек) ---
    if (currentNow - lastTelemetry > 10000) {
        lastTelemetry = currentNow;
        StaticJsonDocument<512> doc;
        
        doc["light_state"] = lightState ? "ON" : "OFF";
        doc["light_move_state"] = lightMoveDetection ? "ON" : "OFF";
        doc["floor_state"] = floorMasterState ? "ON" : "OFF";
        doc["floor_move_state"] = floorMoveDetection ? "ON" : "OFF";
        doc["floor_heat_state"] = floorRelayActual ? "ON" : "OFF";

        // Температуры ( null при ошибках )
        if (floorTemp <= -120.0 || floorTemp == 0) doc["floor_temp"] = nullptr; 
        else doc["floor_temp"] = (float)((int)(floorTemp * 10)) / 10.0;
        
        if (isnan(airTemp)) doc["air_temp"] = nullptr; 
        else doc["air_temp"] = (float)((int)(airTemp * 10)) / 10.0;
        
        if (isnan(airHum)) doc["air_hum"] = nullptr; 
        else doc["air_hum"] = (float)((int)(airHum * 10)) / 10.0;

        doc["pir_raw"] = pirActive;
        doc["since_move_s"] = lightDiff / 1000;
        doc["uptime"] = currentNow / 1000;
        doc["heap"] = ESP.getFreeHeap();

        char buffer[512];
        serializeJson(doc, buffer);
        client.publish("home/hall/state", buffer);
        
        Serial.printf("[TELE] JSON: %s\n", buffer);
    }
}