#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @brief Represents different types of statements.
 */
enum class Statement {
    Temperature,        ///< Temperature statement
    Weekday,            ///< Weekday statement
    Time,               ///< Time statement
    DayLight,           ///< Daylight statement
    ElectricityPrice,   ///< Electricity price statement
    Temperature3h,      ///< Temperature for 3 hours statement
    Temperature6h,      ///< Temperature for 6 hours statement
    Unknown             ///< Unknown statement
};

/**
 * Prints the statements.
 * This function is responsible for printing the statements.
 */
extern void print_statements();

/**
 * @brief The Facts class represents a collection of facts and their corresponding values.
 */
class Facts {
private:
    std::map<Statement, float> facts;

public:
    /**
     * @brief Adds a fact with the specified value to the collection.
     * 
     * @param statement The statement representing the fact.
     * @param value The value associated with the fact.
     */
    void addFact(Statement statement, float value);

    /**
     * @brief Checks if the specified fact exists in the collection.
     * 
     * @param statement The statement representing the fact.
     * @return true if the fact exists, false otherwise.
     */
    bool isFact(Statement statement) const;

    /**
     * @brief Retrieves the value associated with the specified fact.
     * 
     * @param statement The statement representing the fact.
     * @return The value associated with the fact.
     */
    float getValue(Statement statement);
};

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

/**
 * @brief Represents a rule in the expert system.
 */
class Rule {
public:
    std::vector<Condition*> conditions; /**< The conditions of the rule. */
    std::string target; /**< The target of the rule. */
    std::string action; /**< The action of the rule. */

    Rule() = default;


    Rule(std::string target, std::string action) :
        target(target), 
        action(action) 
    {}

    Rule* addRange(Statement statement, float lowerBound, float upperBound) {
        conditions.push_back(new RangeCondition(statement, lowerBound, upperBound));
        return this;
    }

    /**
     * @brief Checks if the conditions of the rule are true based on the given facts.
     */
    bool isConditionTrue(Facts& facts) const;

    void load(const json& obj);
    void save(json& obj) const;
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

    ExpertSystem& addFact(Statement statement, float value) {
        facts.addFact(statement, value);
        return *this;
    }

    std::pair<std::string, std::string> infer();
    void loadRules(const std::string& filename);
    void saveRules(const std::string& filename) const;
};
