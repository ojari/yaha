#include <mosquitto.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include "mqtt_mosquitto.hpp"
#include "common.hpp"
using json = nlohmann::json;


void MessageRouter::route(std::string& deviceName, std::string& payload) {
    std::shared_ptr<device::Device> device = deviceRegistry->getDevice(deviceName);
    if (device) {
        if (verbose) {
            spdlog::info("Device: {} Payload: {}", deviceName, payload);
        }

        json jsonPayload = json::parse(payload);
        device->on_message(deviceName, jsonPayload);
    } else {
        spdlog::error("Missing topic: {} Payload: {}", deviceName, payload);
    }
}


void on_connect(struct mosquitto *mosq, void *obj, int result) {
    if (!result) {
        spdlog::info("Connected");
        mosquitto_subscribe(mosq, NULL, "zigbee2mqtt/+", 0);
    } else {
        spdlog::error("Connect failed: {}", result);
    }
}

void on_disconnect(struct mosquitto *mosq, void *obj, int rc) {
    spdlog::warn("Disconnected: {}", rc);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
    auto* router = static_cast<MessageRouter*>(obj);
    char **topics;
    int topic_count;
    int rc;

    //spdlog::info("Received message on topic: {} with payload: {}", message->topic, (char*)message->payload);

    rc = mosquitto_sub_topic_tokenise(message->topic, &topics, &topic_count);
    if ((rc == MOSQ_ERR_SUCCESS) && (topic_count > 1)) {
        std::string topic = topics[1];
        std::string payload = (char*)message->payload;

        router->route(topic, payload);
    }
    mosquitto_sub_topic_tokens_free(&topics, topic_count);
}

void on_logging(struct mosquitto *mosq, void *obj, int level, const char *str) {
    spdlog::info("MQTT: {} {}", level, str);
}

//------------------------------------------------------------------
Mqtt::Mqtt() : 
    messageRouter(&deviceRegistry)
{
    int rc = 0;
    const char* hostname = getenv("RPI_HOST");

    deviceRegistry.load("devices.json");

    if (hostname == nullptr) {
        spdlog::error("Missing RPI_HOST environmental variable");
        return;
    }

    mosquitto_lib_init();
    mosq = mosquitto_new("yaha", true, static_cast<void*>(&messageRouter));
    if (!mosq) {
        spdlog::error("Out of memory");
        return;
    }

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);
    mosquitto_disconnect_callback_set(mosq, on_disconnect);
    mosquitto_log_callback_set(mosq, on_logging);

    rc = mosquitto_connect(mosq, hostname, 1883, 60);
    if (rc) {
        spdlog::error("Unable to connect: {}", mosquitto_strerror(rc));
        return;
    }
}

void Mqtt::execute() {
    int rc = 0;

    rc = mosquitto_loop(mosq, 5, 1);
    if (rc) {
        spdlog::error("Loop error: {}", mosquitto_strerror(rc));
        mosquitto_reconnect(mosq);
    }
}

void Mqtt::send(const std::string& topic, const std::string& payload) {
    int rc = mosquitto_publish(mosq, NULL, topic.c_str(), payload.size(), payload.c_str(), 0, false);
    if (rc) {
        spdlog::error("Publish error: {}", mosquitto_strerror(rc));
    }
}
