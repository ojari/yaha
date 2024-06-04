#include "tables.hpp"

void dataToHeader(DataHeader& header, const ConfigDevice& config) {
    header.setValue("name", config.name);
    header.setValue("type", config.type);
}

void dataFromHeader(DataHeader& header, ConfigDevice& config) {
    config.name = header.getValue<std::string>("name");
    config.type = header.getValue<std::string>("type"); 
}

//-----------------------------------------------------------------------------
void dataToHeader(DataHeader& header, const ConfigController& config) {
    header.setValue("name", config.name);
    header.setValue("type", config.type);
    header.setValue("actuator", config.actuator);
    header.setValue("time1", config.time1);
    header.setValue("time2", config.time2);
}

void dataFromHeader(DataHeader& header, ConfigController& config) {
    config.name = header.getValue<std::string>("name");
    config.type = header.getValue<std::string>("type");
    config.actuator = header.getValue<std::string>("actuator");
    config.time1 = header.getValue<int>("time1");
    config.time2 = header.getValue<int>("time2");
}

//-----------------------------------------------------------------------------
void dataToHeader(DataHeader& header, const DataTemperature& data) {
    header.setValue("epoch", data.epoch);
    header.setValue("temperature", data.temperature);
    header.setValue("humidity", data.humidity);
}
void dataFromHeader(DataHeader& header, DataTemperature& data) {
    data.epoch = header.getValue<int>("epoch");
    data.temperature = header.getValue<float>("temperature");
    data.humidity = header.getValue<float>("humidity");
}

//-----------------------------------------------------------------------------
void dataToHeader(DataHeader& header, const DataWeather& data) {
    header.setValue("epoch", data.epoch);
    header.setValue("temperature", data.temperature);
    header.setValue("humidity", data.humidity);
    header.setValue("pressure", data.pressure);
    header.setValue("windSpeed", data.windSpeed);
    header.setValue("windDirection", data.windDirection);
    header.setValue("rain", data.rain);
    header.setValue("uv", data.uv);
    header.setValue("solarRadiation", data.solarRadiation);
}

void dataFromHeader(DataHeader& header, DataWeather& data) {
    data.epoch = header.getValue<int>("epoch");
    data.temperature = header.getValue<float>("temperature");
    data.humidity = header.getValue<float>("humidity");
    data.pressure = header.getValue<float>("pressure");
    data.windSpeed = header.getValue<float>("windSpeed");
    data.windDirection = header.getValue<float>("windDirection");
    data.rain = header.getValue<float>("rain");
    data.uv = header.getValue<float>("uv");
    data.solarRadiation = header.getValue<float>("solarRadiation");
}

//-----------------------------------------------------------------------------
void dataToHeader(DataHeader& header, const DataElPrice& data) {
    header.setValue("epoch", data.epoch);
    header.setValue("price", data.price);
}
void dataFromHeader(DataHeader& header, DataElPrice& data) {
    data.epoch = header.getValue<int>("epoch");
    data.price = header.getValue<float>("price");
}

//-----------------------------------------------------------------------------
void dataToHeader(DataHeader& header, const DataHistory& data) {
    header.setValue("epoch", data.epoch);
    header.setValue("device", data.device);
    header.setValue("type", static_cast<int>(data.type));
    header.setValue("val1", data.val1);
    header.setValue("val2", data.val2);
    header.setValue("val3", data.val3);
}
void dataFromHeader(DataHeader& header, DataHistory& data) {
    data.epoch = header.getValue<int>("epoch");
    data.device = header.getValue<std::string>("device");
    data.type = static_cast<DataType>(header.getValue<int>("type"));
    data.val1 = header.getValue<int>("val1");
    data.val2 = header.getValue<int>("val2");
    data.val3 = header.getValue<int>("val3");
}

//-----------------------------------------------------------------------------
