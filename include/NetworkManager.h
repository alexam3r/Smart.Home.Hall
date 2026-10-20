#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

#include <cstdint>

class IMqttCommandSink {
public:
    virtual ~IMqttCommandSink() = default;
    virtual void onLightPower(bool on) = 0;
    virtual void onLightBrightness(uint8_t percent) = 0;
    virtual void onLightMotionMode(bool on) = 0;
    virtual void onFloorPower(bool on) = 0;
    virtual void onFloorMotionMode(bool on) = 0;
    virtual void onFloorTargetTemp(float targetC) = 0;
};

class NetworkManager {
public:
    NetworkManager(IMqttCommandSink& sink, const char* ssid, const char* wifiPassword,
                   const char* mqttUser, const char* mqttPassword, const char* clientId);

    void begin();
    void update(uint32_t now);

    bool connected();
    bool publish(const char* topic, const char* payload, bool retained);

private:
    static void callbackTrampoline(char* topic, byte* payload, unsigned int length);
    void handleMessage(char* topic, byte* payload, unsigned int length);
    void subscribeAll();
    void publishInitialStates();

    IMqttCommandSink& sink_;
    const char* ssid_;
    const char* wifiPassword_;
    const char* mqttUser_;
    const char* mqttPassword_;
    const char* clientId_;

    WiFiClient wifiClient_;
    PubSubClient client_;

    bool initialPublished_ = false;
    float lastTargetC_ = 30.0f;
    bool attempted_ = false;
    bool lost_ = false;
    uint32_t lostAtMs_ = 0;
    uint32_t lastAttemptMs_ = 0;
    uint32_t lastWifiAttemptMs_ = 0;

    static NetworkManager* instance_;
};
