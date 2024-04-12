#include <iostream>
#include <map>
#include <fstream>
#include <sstream> // Include the necessary header file
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include "expert.hpp"


std::unordered_map<std::string, Statement> str2statement = {
    {"Weekday", Statement::Weekday},
    {"Winter", Statement::Winter},
    {"Summer", Statement::Summer},
    {"Day", Statement::Day},
    {"Night", Statement::Night},
    {"ElPriceHigh", Statement::ElPriceHigh},
    {"ElPriceLow", Statement::ElPriceLow},
    {"TempHigh", Statement::TempHigh},
    {"TempLow", Statement::TempLow},
    {"TempLowLow", Statement::TempLowLow},
    {"Temp8hLow", Statement::Temp8hLow},
    {"Temp8hLowLow", Statement::Temp8hLowLow},
    {"Temp24hLow", Statement::Temp24hLow},
    {"Temp24hLowLow", Statement::Temp24hLowLow},
    {"Unknown", Statement::Unknown}
};


std::unordered_map<std::string, ScalarStatement> str2scalarstatement = {
    {"Temperature", ScalarStatement::Temperature},
    {"Time", ScalarStatement::Time},
    {"Unknown", ScalarStatement::Unknown}
};

std::unordered_map<std::string, Weekday> str2weekday = {
    {"Mon", Weekday::Mon},
    {"Tue", Weekday::Tue},
    {"Wed", Weekday::Wed},
    {"Thu", Weekday::Thu},
    {"Fri", Weekday::Fri},
    {"Sat", Weekday::Sat},
    {"Sun", Weekday::Sun}
};

template <typename Enum>
Enum str2enum(const std::string& str, const std::unordered_map<std::string, Enum>& map) {
    auto it = map.find(str);
    if (it != map.end())
        return it->second;
    else
        throw std::invalid_argument("Unknown enum value: " + str);
}

template <typename Enum>
std::string enum2str(Enum value, const std::unordered_map<std::string, Enum>& map) {
    for (const auto& pair : map) {
        if (pair.second == value) {
            return pair.first;
        }
    }
    throw std::invalid_argument("Unknown enum value: " + std::to_string(static_cast<int>(value)));
}

void print_statements() {
    for (const auto& pair : str2statement) {
        std::cout << "    " << int(pair.second) << " - " << pair.first << std::endl;
    }
}

//-----------------------------------------------------------------------------
void Facts::addFact(Statement statement, bool value) {
    facts[statement] = value;
}

void Facts::addFact(ScalarStatement statement, int value) {
    scalarFacts[statement] = value;
}

bool Facts::isFact(ScalarStatement statement) const {
    return scalarFacts.find(statement) != scalarFacts.end();
}

bool Facts::getValue(Statement statement) {
    if (facts.find(statement) != facts.end()) {
        return facts[statement];
    }
    return false;
}

int Facts::getValue(ScalarStatement statement) {
    if (isFact(statement)) {
        return scalarFacts[statement];
    }
    std::cerr << "Fact not found " << int(statement) << std::endl;
    throw std::runtime_error("Fact not found");
}

//-----------------------------------------------------------------------------
void BoolCondition::load(const json& obj) {
    std::string statement_str = obj.get<std::string>();
    statement = str2enum(statement_str, str2statement);
}

void BoolCondition::save(json& obj) const {
    obj.push_back(enum2str(statement, str2statement).c_str());
}

void RangeCondition::load(const json& obj) {
    std::string statement_str = obj[0].get<std::string>();
    statement = str2enum(statement_str, str2scalarstatement);
    lowerBound = obj[1].get<int>();
    upperBound = obj[2].get<int>();
}

void RangeCondition::save(json& obj) const {
    obj.push_back(enum2str(statement, str2scalarstatement).c_str());
    obj.push_back(lowerBound);
    obj.push_back(upperBound);
}

/*
void TimeCondition::load(const json& obj) {
    std::string weekday_str = obj[0].get<std::string>();
    weekday = str2enum(weekday_str, str2weekday);
    startTime = obj[1].get<int>();
    endTime = obj[2].get<int>();
}

void TimeCondition::save(json& obj) const {
    obj.push_back(enum2str(weekday, str2weekday).c_str());
    obj.push_back(startTime);
    obj.push_back(endTime);
}
*/

//-----------------------------------------------------------------------------
std::string Rule::isConditionTrue(Facts& facts) const {
    for (const auto& condition : conditions) {
        // All conditions must pass
        //
        if (!timeMode && !condition->isTrue(facts)) {
            return "";
        }
        // At least one condition must pass
        //
        if (timeMode && condition->isTrue(facts)) {
            return action;
        }
    }
    if (timeMode) {
        return action_off;
    }
    return action;
}

void Rule::execute(ExecutorBase* executor) const {
    executor->execute(target, action);
}

void Rule::load(const json& obj) {
    auto conditionFactory = ConditionFactory();
    for (auto& element : obj.items()) {
        // std::string key = element.key();
        if (element.key() == "then") {
            json thenArray = element.value();
            target = thenArray[0].get<std::string>();
            action = thenArray[1].get<std::string>();
            if (thenArray.size() > 2) {
                action_off = thenArray[2].get<std::string>();
            } 
        }
        else {
            json conditionsArray = element.value();
            for (const auto& conditionObj : conditionsArray) {
                if (element.key() == "time") {
                    timeMode = true;
                }
                auto condition = conditionFactory.make_cond(element.key());
                if (condition != nullptr) {
                    condition->load(conditionObj);
                    conditions.push_back(std::move(condition));
                }
            }            
        }
    }
}

void Rule::save(json& obj) const {
    json ifArr = json::array();
    for (const auto& condition : conditions) {
        json conditionObj = json::array();
        condition->save(conditionObj);
        ifArr.push_back(conditionObj);
    }
    obj["if"] = ifArr;

    json thenArr = json::array();
    thenArr.push_back(target);
    thenArr.push_back(action);
    if (action_off.length() > 0)
        thenArr.push_back(action_off);
    obj["then"] = thenArr;
}


//-----------------------------------------------------------------------------
void ExpertSystem::infer(ExecutorBase& executor) {
    for (auto& rule : rules) {
        std::string action = rule->isConditionTrue(facts);
        if (!action.empty()) {
            executor.execute(rule->getTarget(), action);
        }
    }
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
