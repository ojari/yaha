#include <iostream>
#include <map>
#include <fstream>
#include <sstream> // Include the necessary header file
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include "expert.hpp"


std::unordered_map<std::string, Statement> str2statementMap = {
    {"Temperature", Statement::Temperature},
    {"Weekday", Statement::Weekday},
    {"Time", Statement::Time},
    {"DayLight", Statement::DayLight},
    {"ElectricityPrice", Statement::ElectricityPrice},
    {"Temperature3h", Statement::Temperature3h},
    {"Temperature6h", Statement::Temperature6h}
};

Statement str2statement(const std::string& str) {
    if (str2statementMap.find(str) != str2statementMap.end())
        return str2statementMap[str];
    else
        throw std::invalid_argument("Unknown statement: " + str);
}

std::string statement2str(Statement statement) {
    for (const auto& pair : str2statementMap) {
        if (pair.second == statement) {
            return pair.first;
        }
    }
    throw std::invalid_argument("Unknown statement: " + std::to_string(static_cast<int>(statement)));
}

void print_statements()
{
    for (const auto& pair : str2statementMap) {
        std::cout << "    " << int(pair.second) << " - " << pair.first << std::endl;
    }

}

//-----------------------------------------------------------------------------
void Facts::addFact(Statement statement, float value) {
    facts[statement] = value;
}

bool Facts::isFact(Statement statement) const {
    return facts.find(statement) != facts.end();
}

float Facts::getValue(Statement statement) {
    if (isFact(statement)) {
        return facts[statement];
    }
    std::cerr << "Fact not found " << int(statement) << std::endl;
    throw std::runtime_error("Fact not found");
}

//-----------------------------------------------------------------------------
void Condition::load(const json& obj) {
    std::string statement_str = obj[0].get<std::string>();
    statement = str2statement(statement_str);
    lowerBound = obj[1].get<float>();
    upperBound = obj[2].get<float>();
}

void Condition::save(json& obj) const {
    obj.push_back(statement2str(statement).c_str());
    obj.push_back(lowerBound);
    obj.push_back(upperBound);
}


//-----------------------------------------------------------------------------
bool Rule::isConditionTrue(Facts& facts) const {
    for (const auto& condition : conditions) {
        float value = facts.getValue(condition.statement);
        if (value < condition.lowerBound || value > condition.upperBound) {
            return false;
        }
    }
    return true;
}

void Rule::load(const json& obj) {
    json conditionsArray = obj["if"];
    for (const auto& conditionObj : conditionsArray) {
        Condition condition;
        condition.load(conditionObj);
        conditions.push_back(condition);
    }
    json thenArray = obj["then"];
    target = thenArray[0].get<std::string>();
    action = thenArray[1].get<std::string>();
}

void Rule::save(json& obj) const {
    json conditionsArray = json::array();
    for (const auto& condition : conditions) {
        json conditionObj = json::array();
        condition.save(conditionObj);
        conditionsArray.push_back(conditionObj);
    }
    obj["conditions"] = conditionsArray;
    obj["target"] = target;
    obj["action"] = action;
}


//-----------------------------------------------------------------------------
std::pair<std::string, std::string> ExpertSystem::infer() {
    for (auto& rule : rules) {
        if (rule->isConditionTrue(facts)) {
            // return std::make_pair(rule->target, rule->action);
            std::cout << rule->target << "::" << rule->action << "\n";
        }
    }
    return std::make_pair("NONE", "unknown");
}

void ExpertSystem::loadRules(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    json root;
    file >> root;

    for (const auto& ruleObj : root) {
        std::unique_ptr<Rule> rule = std::make_unique<Rule>();
        rule->load(ruleObj);
        addRule(std::move(rule));
    }
}

void ExpertSystem::saveRules(const std::string& filename) const {
    json root = json::array();
    for (const auto& rule : rules) {
        json ruleObj = json::object();
        rule->save(ruleObj);
        root.push_back(ruleObj);
    }

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file << root.dump(2);
}
