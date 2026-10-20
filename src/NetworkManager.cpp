#include "NetworkManager.h"

#include <ESP8266WiFi.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Config.h"
#include "MqttTopics.h"

NetworkManager* NetworkManager::instance_ = nullptr;

NetworkManager::NetworkManager(IMqttCommandSink& sink, const char* ssid,
                               const char* wifiPassword, const char* mqttUser,
                               const char* mqttPassword, const char* clientId)
    : sink_(sink),
      ssid_(ssid),
      wifiPassword_(wifiPassword),
      mqttUser_(mqttUser),
      mqttPassword_(mqttPassword),
      clientId_(clientId),
      client_(wifiClient_) {}

void NetworkManager::begin() {
    instance_ = this;

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_, wifiPassword_);

    client_.setCallback(callbackTrampoline);
    client_.setBufferSize(512);
    // Ограничиваем блокировку loop() на время TCP-connect.
    wifiClient_.setTimeout(2000);
}

void NetworkManager::update(uint32_t now) {
    client_.loop();

    const bool wifiUp = WiFi.status() == WL_CONNECTED;

    if (!wifiUp) {
        if (!lost_) {
            lost_ = true;
            lostAtMs_ = now;
        }
        if (static_cast<uint32_t>(now - lostAtMs_) >= cfg::MQTT_LOST_REBOOT_MS) {
            ESP.restart();
        }
        if (static_cast<uint32_t>(now - lastWifiAttemptMs_) >= cfg::MQTT_RETRY_MS) {
            lastWifiAttemptMs_ = now;
            WiFi.reconnect();
        }
        return;
    }

    if (!client_.connected()) {
        if (!lost_) {
            lost_ = true;
            lostAtMs_ = now;
        }
        if (static_cast<uint32_t>(now - lostAtMs_) >= cfg::MQTT_LOST_REBOOT_MS) {
            ESP.restart();
        }
        if (!attempted_ ||
            static_cast<uint32_t>(now - lastAttemptMs_) >= cfg::MQTT_RETRY_MS) {
            attempted_ = true;
            lastAttemptMs_ = now;

            client_.setServer(WiFi.gatewayIP(), 1883);
            if (client_.connect(clientId_, mqttUser_, mqttPassword_)) {
                lost_ = false;
                subscribeAll();
                if (!initialPublished_) {
                    publishInitialStates();
                    initialPublished_ = true;
                }
            }
        }
    } else {
        lost_ = false;
    }
}

bool NetworkManager::connected() {
    return WiFi.status() == WL_CONNECTED && client_.connected();
}

bool NetworkManager::publish(const char* topic, const char* payload, bool retained) {
    if (!client_.connected()) {
        return false;
    }
    return client_.publish(topic, payload, retained);
}

void NetworkManager::callbackTrampoline(char* topic, byte* payload, unsigned int length) {
    if (instance_ != nullptr) {
        instance_->handleMessage(topic, payload, length);
    }
}

void NetworkManager::handleMessage(char* topic, byte* payload, unsigned int length) {
    char message[32] = {0};
    const unsigned int copyLength =
        length < sizeof(message) - 1 ? length : sizeof(message) - 1;
    memcpy(message, payload, copyLength);

    const bool on = strcmp(message, "ON") == 0;

    if (strcmp(topic, mqtt::LIGHT_CMD) == 0) {
        sink_.onLightPower(on);
    } else if (strcmp(topic, mqtt::LIGHT_BRIGHTNESS_CMD) == 0) {
        long percent = atol(message);
        if (percent < 1) {
            percent = 1;
        } else if (percent > 100) {
            percent = 100;
        }
        sink_.onLightBrightness(static_cast<uint8_t>(percent));
    } else if (strcmp(topic, mqtt::LIGHT_MOTION_CMD) == 0) {
        sink_.onLightMotionMode(on);
        client_.publish(mqtt::LIGHT_MOTION_STATE, on ? "ON" : "OFF", true);
    } else if (strcmp(topic, mqtt::FLOOR_CMD) == 0) {
        sink_.onFloorPower(on);
    } else if (strcmp(topic, mqtt::FLOOR_MOTION_CMD) == 0) {
        sink_.onFloorMotionMode(on);
        client_.publish(mqtt::FLOOR_MOTION_STATE, on ? "ON" : "OFF", true);
    } else if (strcmp(topic, mqtt::FLOOR_TARGET) == 0 ||
               strcmp(topic, mqtt::FLOOR_TARGET_LEGACY) == 0) {
        const float targetC = static_cast<float>(atof(message));
        sink_.onFloorTargetTemp(targetC);

        // Держим оба топика в синхронизации, чтобы старые HA-конфиги
        // и новый канонический targetTemp видели одну уставку.
        char targetPayload[16] = {0};
        snprintf(targetPayload, sizeof(targetPayload), "%.1f", targetC);
        client_.publish(mqtt::FLOOR_TARGET, targetPayload, true);
        client_.publish(mqtt::FLOOR_TARGET_LEGACY, targetPayload, true);
    }
}

void NetworkManager::subscribeAll() {
    client_.subscribe(mqtt::LIGHT_CMD);
    client_.subscribe(mqtt::LIGHT_BRIGHTNESS_CMD);
    client_.subscribe(mqtt::LIGHT_MOTION_CMD);
    client_.subscribe(mqtt::FLOOR_CMD);
    client_.subscribe(mqtt::FLOOR_MOTION_CMD);
    client_.subscribe(mqtt::FLOOR_TARGET);
    client_.subscribe(mqtt::FLOOR_TARGET_LEGACY);
}

void NetworkManager::publishInitialStates() {
    client_.publish(mqtt::LIGHT_STATE, "OFF", true);
    client_.publish(mqtt::LIGHT_BRIGHTNESS_STATE, "100", true);
    client_.publish(mqtt::LIGHT_MOTION_STATE, "OFF", true);
    client_.publish(mqtt::FLOOR_STATE, "OFF", true);
    client_.publish(mqtt::FLOOR_HEAT_STATE, "OFF", true);
    client_.publish(mqtt::FLOOR_MOTION_STATE, "OFF", true);
    client_.publish(mqtt::FLOOR_TARGET, "30.0", true);
    client_.publish(mqtt::FLOOR_TARGET_LEGACY, "30.0", true);
    client_.publish(mqtt::LIGHT_MOVE, "OFF", false);
}
