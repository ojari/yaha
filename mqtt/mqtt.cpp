#include <mosquitto.h>
#include <iostream>
#include <string>
#include "mqtt.hpp"
#include "common.hpp"


void MessageRouter::route(std::string& deviceName, std::string& payload) {
    Device* device = deviceRegistry->getDevice(deviceName);
    if (device) {
        device->on_message(deviceName, payload);
    }
}


void on_connect(struct mosquitto *mosq, void *obj, int result) {
    if (!result) {
        //mosquitto_subscribe(mosq, NULL, "zigbee2mqtt/+/state", 0);
        mosquitto_subscribe(mosq, NULL, "zigbee2mqtt/+", 0);
    } else {
        std::cerr << "Connect failed\n";
    }
}


void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
    MessageRouter* router = static_cast<MessageRouter*>(obj);
    char **topics;
    int topic_count;
    int rc;

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

    deviceRegistry.registerDevice("light", &lightDevice);
    deviceRegistry.registerDevice("thermostat", &thermostatDevice);

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

void Mqtt::loop() {
    int rc = 0;

    while (true) {
        rc = mosquitto_loop(mosq, -1, 1);
        if (rc) {
            showError(mosquitto_strerror(rc));
            // sleep(10);
            mosquitto_reconnect(mosq);
        }
    }
}
