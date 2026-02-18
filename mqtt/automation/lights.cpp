#include "lights.hpp"
#include <map>
#include <stdexcept>

namespace automation {

Lights::Lights(std::shared_ptr<IOutput> output, const std::string& name) :
    Automation(std::move(output), name)
{
    initial_value(false);
}

Lights::TimeCondition::TimeCondition(int onTime, int offTime) :
    Condition(),
    onTime(onTime),
    offTime(offTime)
{
}

bool Lights::TimeCondition::matches(const Facts& facts) const {
    const int current = facts.getTime();
    if (current < 0) {
        return false;
    }

    if (onTime <= offTime) {
        return current >= onTime && current < offTime;
    }

    return current >= onTime || current < offTime;
}

Lights::DarkCondition::DarkCondition(bool whenDark) :
    Condition(),
    whenDark(whenDark)
{
}

bool Lights::DarkCondition::matches(const Facts& facts) const {
    return facts.isDark() == whenDark;
}

Lights::ElectricityPriceCondition::ElectricityPriceCondition(double threshold, bool above) :
    Condition(),
    threshold(threshold),
    above(above)
{
}

bool Lights::ElectricityPriceCondition::matches(const Facts& facts) const {
    const double value = facts.getElectricityPrice();
    if (above) {
        return value >= threshold;
    }
    return value <= threshold;
}

//------------------------------------------------------------------------------------------------
void Lights::setCondition(const std::string &type, const nlohmann::json& data) {
    if (type == "time") {
        if (!data.contains("on") || !data.contains("off")) {
            throw std::runtime_error("Time condition requires 'on' and 'off' fields");
        }
        conditions.push_back(std::make_unique<TimeCondition>(data["on"].get<int>(), data["off"].get<int>()));
    } else if (type == "dark") {
        if (!data.contains("whenDark")) {
            throw std::runtime_error("Dark condition requires 'whenDark' field");
        }
        conditions.push_back(std::make_unique<DarkCondition>(data["whenDark"].get<bool>()));
    } else if (type == "electricityPrice") {
        if (!data.contains("threshold") || !data.contains("above")) {
            throw std::runtime_error("Electricity price condition requires 'threshold' and 'above' fields");
        }
        conditions.push_back(std::make_unique<ElectricityPriceCondition>(data["threshold"].get<double>(), data["above"].get<bool>()));
    } else {
        throw std::runtime_error("Unknown condition type: " + type);
    }
}


void Lights::setArg(const std::string& name, const std::string& value) {
    if (name == "brightness") {
        brightness = std::stoi(value);
    } else {
        throw std::runtime_error("Unknown argument: " + name);
    }
}

void Lights::registerEvents(EventBus& evbus) {
    evbus.subscribe<TimeEvent>([this](const TimeEvent& event) { onEvent(event); });
    evbus.subscribe<DateEvent>([this](const DateEvent& event) { onEvent(event); });
    evbus.subscribe<DarkEvent>([this](const DarkEvent& event) { onEvent(event); });
    evbus.subscribe<ElectricityPriceEvent>([this](const ElectricityPriceEvent& event) { onEvent(event); });
}

void Lights::onEvent(const TimeEvent& event) {
    facts.setTime(event.GetTime());

    updateState();
}

void Lights::onEvent(const DateEvent& event) {
    facts.setWeekDay(event.weekday);
    facts.setDate(event.day, event.month, event.year);
}

void Lights::onEvent(const DarkEvent& event) {
    facts.setDark(event.dark);
}

void Lights::onEvent(const ElectricityPriceEvent& event) {
    facts.setElectricityPrice(event.price);
}

void Lights::updateState() {
    if (conditions.empty()) {
        send(true);
        return;
    }

    std::map<Condition::Group, bool> groupSatisfied;
    for (const auto& cond : conditions) {
        const auto group = cond->group();
        const bool matches = cond->matches(facts);
        auto [it, inserted] = groupSatisfied.emplace(group, matches);
        if (!inserted && matches) {
            it->second = true;
        }
    }

    for (const auto& [group, satisfied] : groupSatisfied) {
        if (!satisfied) {
            send(false);
            return;
        }
    }

    send(true);
}

}  // namespace automation
