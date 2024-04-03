#include <mosquitto.h>
#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "mqtt.hpp"
#include "common.hpp"
using json = nlohmann::json;


void MessageRouter::route(std::string& deviceName, std::string& payload) {
    Device* device = deviceRegistry->getDevice(deviceName);
    if (device) {
        device->on_message(deviceName, payload);
    }
}


void on_connect(struct mosquitto *mosq, void *obj, int result) {
    if (!result) {
        std::cout << "Connected" << std::endl;
        //mosquitto_subscribe(mosq, NULL, "zigbee2mqtt/+/state", 0);
        mosquitto_subscribe(mosq, NULL, "zigbee2mqtt/+", 0);
    } else {
        std::cerr << "Connect failed: " << result << std::endl;
    }
}


void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
    MessageRouter* router = static_cast<MessageRouter*>(obj);
    char **topics;
    int topic_count;
    int rc;

    // std::cout << "Topic: " << message->topic << " Message: " << (char*)message->payload << std::endl;

    rc = mosquitto_sub_topic_tokenise(message->topic, &topics, &topic_count);
    if ((rc == MOSQ_ERR_SUCCESS) && (topic_count > 1)) {
        std::string topic = topics[1];
        std::string payload = (char*)message->payload;

        router->route(topic, payload);
    }
    mosquitto_sub_topic_tokens_free(&topics, topic_count);
}


//------------------------------------------------------------------
Mqtt::Mqtt() : 
    messageRouter(&deviceRegistry)
{
    int rc = 0;
    char* hostname = getenv("RPI_HOST");

    readDevices();

    if (hostname == NULL) {
        showError("Missing RPI_HOST environmental variable");
        return;
    }

    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, static_cast<void*>(&messageRouter));
    if (!mosq) {
        showError("Out of memory");
        return;
    }

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    rc = mosquitto_connect(mosq, hostname, 1883, 60);
    if (rc) {
        showError(mosquitto_strerror(rc));
        return;
    }
}

void Mqtt::readDevices() {
    std::ifstream file("devices.json");
    if (!file) {
        showError("Error opening devices.json");
        return;
    }

    json root;
    file >> root;

    for (const auto& deviceData : root) {
        std::string name = deviceData["name"].get<std::string>();
        std::string type = deviceData["type"].get<std::string>();

        if (type == "Lamp") {
            deviceRegistry.registerDevice(name, &lightDevice);
        } else if (type == "Thermostat") {
            deviceRegistry.registerDevice(name, &thermostatDevice);
        }
        // std::cout << "Device: " << name << " Type: " << type << std::endl;
    }
}

void Mqtt::loop() {
    int rc = 0;

    rc = mosquitto_loop(mosq, -1, 1);
    if (rc) {
        showError(mosquitto_strerror(rc));
        // sleep(10);
        mosquitto_reconnect(mosq);
    }
}
