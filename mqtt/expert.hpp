#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "es_cond.hpp"


/**
 * Prints the statements.
 * This function is responsible for printing the statements.
 */
extern void print_statements();


/**
 * @brief Represents a rule in the expert system.
 */
class Rule {
public:
    Rule() = default;


    Rule(std::string target, std::string action) :
        target(target), 
        action(action) 
    {}

    Rule* addRange(Statement statement, long lowerBound, long upperBound) {
        conditions.push_back(new RangeCondition(statement, lowerBound, upperBound));
        return this;
    }

    Rule* addTime(Weekday weekday, long startTime, long endTime) {
        conditions.push_back(new TimeCondition(weekday, startTime, endTime));
        return this;
    }

    /**
     * @brief Checks if the conditions of the rule are true based on the given facts.
     * 
     * @return action name of empty string if no action is to be taken.
     */
    std::string isConditionTrue(Facts& facts) const;
    void execute(ExecutorBase* executor) const;

    void load(const json& obj);
    void save(json& obj) const;

    std::string getTarget() const {
        return target;
    }

private:
    std::vector<Condition*> conditions; /**< The conditions of the rule. */
    std::string target; /**< The target of the rule. */
    std::string action; /**< The action of the rule. */
    std::string action_off;
    bool timeMode = false;
};

class ExpertSystem {
private:
    std::vector<std::unique_ptr<Rule>> rules;
    Facts facts;

public:
    ExpertSystem& addRule(std::unique_ptr<Rule> rule, ExecutorBase* executor = nullptr) {
        rules.push_back(std::move(rule));
        return *this;
    }

    ExpertSystem& addFact(Statement statement, long value) {
        facts.addFact(statement, value);
        return *this;
    }

    void infer(ExecutorBase& executor);
    void loadRules(const std::string& filename);
    void saveRules(const std::string& filename) const;
};
