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

    void addCondition(std::unique_ptr<Condition> condition) {
        conditions.push_back(std::move(condition));
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
    std::vector<std::unique_ptr<Condition>> conditions; /**< The conditions of the rule. */
    std::string target; /**< The target of the rule. */
    std::string action; /**< The action of the rule. */
    std::string action_off;
    bool timeMode = false;
};

// @pattern builder
//
class RuleBuilder {
public:
    RuleBuilder(std::string target, std::string action)
    {
        root = std::make_unique<Rule>(target, action);
    }

    RuleBuilder& addRange(Statement statement, long lowerBound, long upperBound) {
        root->addCondition(std::make_unique<RangeCondition>(statement, lowerBound, upperBound));
        return *this;
    }

    RuleBuilder& addTime(Weekday weekday, long startTime, long endTime) {
        root->addCondition(std::make_unique<TimeCondition>(weekday, startTime, endTime));
        return *this;
    }

    std::unique_ptr<Rule> getRule() {
        return std::move(root);
    }

private:
    std::unique_ptr<Rule> root;
};

class ExpertSystem {
private:
    std::vector<std::unique_ptr<Rule>> rules;
    Facts facts;

public:
    ExpertSystem& addRule(std::unique_ptr<Rule> rule) {
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
