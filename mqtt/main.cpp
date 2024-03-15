#include "expert.hpp"
#include <iostream>

/**
 * @brief Interacts with the expert system by adding facts and getting the inferred action.
 */
void interact(ExpertSystem& system)
{
    int fact;
    float value;
    std::cout << "Enter a fact:" << std::endl;
    print_statements();

    std::cin >> fact;
    std::cout << "Enter a value: ";
    std::cin >> value;
    std::cin.ignore();  // ignore the newline character
    system.addFact(static_cast<Statement>(fact), value);

    auto result = system.infer();
    std::cout << result.first << "::" << result.second << "\n";
}   

/**
 * Fills the rules of the expert system.
 */
void fill_rules(ExpertSystem& system) {
    std::unique_ptr<Rule> rule = std::make_unique<Rule>("HEATER", "ON");
    rule->addCondition(Statement::Temperature, -30 , 0);
    rule->addCondition(Statement::DayLight, 0 , 1);
    system.addRule(std::move(rule));

    std::unique_ptr<Rule> rule2 = std::make_unique<Rule>("HEADER", "OFF");
    rule2->addCondition(Statement::Temperature, 20 , 90);
    rule2->addCondition(Statement::Weekday, 1 , 1);
    system.addRule(std::move(rule2));

    system.saveRules("rules.json");
 }

/**
 * @brief The main function of the program.
 */
int main()
{
    ExpertSystem system;

    system.addFact(Statement::Temperature, 20);
    system.addFact(Statement::Weekday, 1);
    system.addFact(Statement::Time, 1020);
    system.addFact(Statement::DayLight, 1);
    system.addFact(Statement::ElectricityPrice, 0.1);
    system.addFact(Statement::Temperature3h, 20);
    system.addFact(Statement::Temperature6h, 20);

    // fill_rules(system);
    system.loadRules("rules.json");
    while (true) {
        interact(system);
        //std::this_thread::sleep_for(std::chrono::minutes(1));
    }

    return 0;
}
