#include <iostream>
#include <map>
#include <fstream>
#include <sstream> // Include the necessary header file
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <jansson.h>
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
void Condition::load(json_t* obj) {
    const char* statement_str = json_string_value(json_object_get(obj, "statement"));
    statement = str2statement(statement_str);
    lowerBound = json_number_value(json_object_get(obj, "lower"));
    upperBound = json_number_value(json_object_get(obj, "upper"));
}
void Condition::save(json_t* obj) const {
    json_object_set_new(obj, "statement", json_string(statement2str(statement).c_str()));
    json_object_set_new(obj, "lower", json_real(lowerBound));
    json_object_set_new(obj, "upper", json_real(upperBound));
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

void Rule::load(json_t* obj) {
    json_t* conditionsArray = json_object_get(obj, "conditions");
    size_t index;
    json_t* conditionObj;
    json_array_foreach(conditionsArray, index, conditionObj) {
        Condition condition;
        condition.load(conditionObj);
        conditions.push_back(condition);
    }
    target = json_string_value(json_object_get(obj, "target"));
    action = json_string_value(json_object_get(obj, "action"));
}

void Rule::save(json_t* obj) const {
    json_t* conditionsArray = json_array();
    for (const auto& condition : conditions) {
        json_t* conditionObj = json_object();
        condition.save(conditionObj);
        json_array_append_new(conditionsArray, conditionObj);
    }
    json_object_set_new(obj, "conditions", conditionsArray);
    json_object_set_new(obj, "target", json_string(target.c_str()));
    json_object_set_new(obj, "action", json_string(action.c_str()));
}


//-----------------------------------------------------------------------------
std::pair<std::string, std::string> ExpertSystem::infer() {
    for (auto& rule : rules) {
        if (rule->isConditionTrue(facts)) {
            return std::make_pair(rule->target, rule->action);
        }
    }
    return std::make_pair("NONE", "unknown");
}

void ExpertSystem::loadRules(const std::string& filename) {
    json_error_t error;
    json_t* root = json_load_file(filename.c_str(), 0, &error);
    if (!root) {
        std::cerr << "Error on line " << error.line << ": " << error.text << std::endl;
        return;
    }

    size_t index;
    json_t* ruleObj;
    json_array_foreach(root, index, ruleObj) {
        std::unique_ptr<Rule> rule = std::make_unique<Rule>();
        rule->load(ruleObj);
        addRule(std::move(rule));
    }
    json_decref(root);
}

void ExpertSystem::saveRules(const std::string& filename) const {
    json_t* root = json_array();
    for (const auto& rule : rules) {
        json_t* ruleObj = json_object();
        rule->save(ruleObj);
        json_array_append_new(root, ruleObj);
    }

    json_dump_file(root, filename.c_str(), JSON_INDENT(2));
    json_decref(root);
}
