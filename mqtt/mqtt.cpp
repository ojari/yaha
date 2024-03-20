#include <mosquitto.h>
#include <iostream>
#include <string>
#include <unordered_map>


class Device {
public:
    virtual void on_message(const struct mosquitto_message *message) = 0;
};



class LightDevice : public Device {
public:
    void on_message(const struct mosquitto_message *message) override {
        std::string topic = message->topic;
        std::string payload = (char*) message->payload;

        std::cout << "LightDevice received message on topic: " << topic << " with payload: " << payload << "\n";

    }
};

class ThermostatDevice : public Device {
public:
    void on_message(const struct mosquitto_message *message) override {
        std::string topic = message->topic;
        std::string payload = (char*) message->payload;

        std::cout << "ThermostatDevice received message on topic: " << topic << " with payload: " << payload << "\n";

    }
};

class DeviceRegistry {
public:
    void registerDevice(const std::string& name, Device* device) {
        devices_[name] = device;
    }

    Device* getDevice(const std::string& name) {
        auto it = devices_.find(name);
        if (it != devices_.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, Device*> devices_;
};

class MessageRouter {
public:
    MessageRouter(DeviceRegistry* deviceRegistry) : deviceRegistry_(deviceRegistry) {}

    void route(struct mosquitto *mosq, const struct mosquitto_message *message) {
        std::string topic = message->topic;
        std::string payload = (char*) message->payload;

        // Extract the device name from the topic
        std::string deviceName = extractDeviceName(topic);

        // Get the device from the registry
        Device* device = deviceRegistry_->getDevice(deviceName);

        // If the device exists, forward the message to it
        if (device) {
            device->on_message(message);
        }
    }

private:
    DeviceRegistry* deviceRegistry_;

    std::string extractDeviceName(const std::string& topic) {
        // Extract the device name from the topic
        std::size_t start = topic.find_first_of("/") + 1;
        std::size_t end = topic.find_first_of("/", start);
        return topic.substr(start, end - start);
    }
};


void on_connect(struct mosquitto *mosq, void *obj, int result)
{
    if(!result){
        //mosquitto_subscribe(mosq, NULL, "zigbee2mqtt/+/state", 0);
        mosquitto_subscribe(mosq, NULL, "zigbee2mqtt/+", 0);
    } else {
        std::cerr << "Connect failed\n";
    }
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    MessageRouter* router = static_cast<MessageRouter*>(obj);
    router->route(mosq, message);
}


int task_mqtt()
{
    struct mosquitto *mosq;
    int rc = 0;

    mosquitto_lib_init();

    // Create instances of devices
    LightDevice lightDevice;
    ThermostatDevice thermostatDevice;

    // Create an instance of the device registry
    DeviceRegistry deviceRegistry;

    // Register devices with the registry
    deviceRegistry.registerDevice("light", &lightDevice);
    deviceRegistry.registerDevice("thermostat", &thermostatDevice);

    // Create an instance of the message router
    MessageRouter messageRouter(&deviceRegistry);

    // Rest of the code remains the same

    mosq = mosquitto_new(NULL, true, static_cast<void*>(&messageRouter));
    if(!mosq){
        std::cerr << "Error: Out of memory.\n";
        return 1;
    }

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    rc = mosquitto_connect(mosq, "localhost", 1883, 60);
    if(rc){
        std::cerr << "Unable to connect.\n";
        return 1;
    }

    while(true) {
        rc = mosquitto_loop(mosq, -1, 1);
        if(rc){
	    std::cout << "connection error!" << std::endl;
            // sleep(10);
            mosquitto_reconnect(mosq);
        }
    }

    return rc;
}
