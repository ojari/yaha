#pragma once
#include <string>
#include <nlohmann/json.hpp>


class Controller {
public:
    virtual ~Controller() = default;

    virtual void on_message(std::string& deviceName, nlohmann::json& payload) = 0;
    virtual void getHistory(DataHistory &history) = 0;
protected:
    bool str2bool(const std::string& str) const {
        return str == "ON";
    }
};
