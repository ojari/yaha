#pragma once
#include "es_base.hpp"


struct Condition {
    virtual bool isTrue(Facts& facts) const = 0;
    virtual void load(const json& obj) = 0;
    virtual void save(json& obj) const = 0;
};


struct RangeCondition : public Condition {
    RangeCondition() = default;

    RangeCondition(Statement statement, float lowerBound, float upperBound) :
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
    Statement statement;
    float lowerBound;
    float upperBound;
};


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
