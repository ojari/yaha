#include <MQTTClient.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <algorithm>
#include "mqtt.hpp"
#include "common.hpp"
using json = nlohmann::json;


void MessageRouter::route(std::string& deviceName, std::string& payload) {
    std::shared_ptr<device::Device> device = deviceRegistry->getDevice(deviceName);
    if (device) {
        if (verbose) {
            spdlog::info("Device: {} Payload: {}", deviceName, payload);
        }

        json jsonPayload = json::parse(payload);
        try {
            device->on_message(deviceName, jsonPayload);
        } catch (const nlohmann::json::parse_error& e) {
            spdlog::error("Json parse error: {}", e.what());
        } catch (const nlohmann::json::type_error& e) {
            spdlog::error("Json type error: {}", e.what());
            spdlog::error("Device: {} Payload: {}", deviceName, payload);
        }
    } else {
        spdlog::error("Missing topic: {} Payload: {}", deviceName, payload);
    }
}

void MessageRouter::bridge_msg(std::string& topic, std::string& payload) {
#if 0
    if (topic == "zigbee2mqtt/bridge/devices") {
        // spdlog::info("Bridge log: {}", payload);
        std::ofstream file("payload.json");
        if (file.is_open()) {
            file << payload;
            file.close();
            spdlog::info("Payload saved to payload.json");
        } else {
            spdlog::error("Unable to open file payload.json for writing");
        }
    } else {
        // spdlog::error("Unknown bridge topic: {}", topic);
    }
#endif
}


void mqtt_delivered(void *context, MQTTClient_deliveryToken dt)
{
    // spdlog::info("Message with token value {} delivery confirmed", dt);
    //deliveredtoken = dt;
}

int mqtt_msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    std::string payload {static_cast<char*>(message->payload), static_cast<size_t>(message->payloadlen)};
    std::string topic {topicName};
    int slashCount = std::count(topic.begin(), topic.end(), '/');

    if (topic.find("/bridge") != std::string::npos) {
        auto* router = static_cast<MessageRouter*>(context);
        router->bridge_msg(topic, payload);
        MQTTClient_freeMessage(&message);
        MQTTClient_free(topicName);
        return 1;
    }

    if (slashCount != 1) {
        MQTTClient_freeMessage(&message);
        MQTTClient_free(topicName);
        return 1;
    }
    // spdlog::info("Message arrived");
    // spdlog::info("     topic: {}", topicName);
    // spdlog::info("   message: {}", payload.c_str());

    size_t firstSlash = topic.find('/');
    if (firstSlash != std::string::npos) {
        auto* router = static_cast<MessageRouter*>(context);
        
        std::string device_name = topic.substr(firstSlash + 1);
        router->route(device_name, payload);
    } else {
        spdlog::error("Invalid topic format: {}", topic);
    }
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void mqtt_connlost(void *context, char *cause)
{
    spdlog::error("Connection lost cause: {}", cause);
}

//------------------------------------------------------------------
Mqtt::Mqtt(const std::string& filename) : 
    messageRouter(&deviceRegistry)
{
    int rc = 0;
    const char* hostname = getenv("RPI_HOST");

    deviceRegistry.load(filename);

    if (hostname == nullptr) {
        spdlog::error("Missing RPI_HOST environmental variable");
        return;
    }

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&client, hostname, "yaha", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;

    rc = MQTTClient_setCallbacks(
        client,
        static_cast<void*>(&messageRouter),
        mqtt_connlost,
        mqtt_msgarrvd,
        mqtt_delivered);
    if (rc != MQTTCLIENT_SUCCESS) {
        spdlog::error("Unable to set callbacks: {}", rc);
        return;
    }

    rc = MQTTClient_connect(client, &conn_opts);
    if (rc != MQTTCLIENT_SUCCESS) {
        spdlog::error("Unable to connect: {}", rc);
        return;
    }

    rc = MQTTClient_subscribe(client, "zigbee2mqtt/#", 0);
    if (rc != MQTTCLIENT_SUCCESS) {
        spdlog::error("Unable to subscribe: {}", rc);
        return;
    }
}

void Mqtt::execute() {
    int rc = 0;

/*    rc = mosquitto_loop(mosq, 5, 1);
    if (rc) {
        spdlog::error("Loop error: {}", mosquitto_strerror(rc));
        mosquitto_reconnect(mosq);
    }
*/
}

void Mqtt::send(std::string_view topic, const std::string& payload) {
    int rc = 0;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    // MQTTClient_deliveryToken token;

    pubmsg.payload = const_cast<char*>(payload.c_str());
    pubmsg.payloadlen = payload.size();
    pubmsg.qos = 1;
    pubmsg.retained = 0;

    rc = MQTTClient_publishMessage(client, topic.data(), &pubmsg, NULL);
    if (rc != MQTTCLIENT_SUCCESS) {
        errorCounter++;
        if (errorCounter <= 3) {
            spdlog::error("Unable to publish: {}", rc);
        }
    }
    else {
        errorCounter = 0;
    }
}
