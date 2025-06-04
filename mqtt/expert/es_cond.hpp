#pragma once
#include "es_base.hpp"
#include <memory>
#include <functional>
#include <iostream>

namespace expert {

struct Condition {
    virtual bool isTrue(Facts& facts) const = 0;
    virtual void load(const json& obj) = 0;
    virtual void save(json& obj) const = 0;
};

struct BoolCondition : public Condition {
    BoolCondition() = default;

    explicit BoolCondition(Statement statement) :
        statement(statement)
    {}
    bool isTrue(Facts& facts) const {
        return facts.getValue(statement);
    }
    void load(const json& obj);
    void save(json& obj) const;

private:
    Statement statement;
};


struct RangeCondition : public Condition {
    RangeCondition() = default;

    RangeCondition(ScalarStatement, int lowerBound, int upperBound) :
        statement(statement),
        lowerBound(lowerBound),
        upperBound(upperBound)
    {}
    bool isTrue(Facts& facts) const {
        if (!facts.isFact(statement)) {
            return false;
        }
        float value = facts.getValue(statement);
        return value >= lowerBound && value <= upperBound;
    }
    void load(const json& obj);
    void save(json& obj) const;

private:
    ScalarStatement statement;
    int lowerBound;
    int upperBound;
};

/*
struct TimeCondition : public Condition {
    TimeCondition() = default;

    TimeCondition(Weekday weekday, int startTime, int endTime) :
        weekday(weekday),
        startTime(startTime),
        endTime(endTime)
    {}
    bool isTrue(Facts& facts) const {
        if (facts.getValue(Statement::Weekday) != static_cast<long>(weekday)) {
            return false;
        }
        int time = facts.getValue(Statement::Time);
        return time >= startTime && time <= endTime;
    }
    void load(const json& obj);
    void save(json& obj) const;

private:
    Weekday weekday;
    int startTime;
    int endTime;
};
*/
// @pattern functional factory
//
class ConditionFactory {
    std::map<std::string, std::function<std::unique_ptr<Condition>()>> creators;
public:
    ConditionFactory() {
        creators["if"] = []() { return std::make_unique<BoolCondition>(); };
        creators["range"] = []() { return std::make_unique<RangeCondition>(); };
        // creators["time"] = []() { return std::make_unique<TimeCondition>(); };
    }

    std::unique_ptr<Condition> make_cond(std::string type) {
        if (creators.find(type) == creators.end()) {
            std::cerr << "Invalid condition in rules file " << type << std::endl;
            return nullptr;
        }
        auto result = creators[type]();
        return result;
    }
};

}  // namespace expert
