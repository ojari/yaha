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
    // std::cout << result.first << "::" << result.second << "\n";
}   

/**
 * Fills the rules of the expert system.
 */
void fill_rules(ExpertSystem& system) {
    std::unique_ptr<Rule> rule = std::make_unique<Rule>("HEATER", "ON");
    rule->addRange(Statement::Temperature, -30 , 0)
        ->addRange(Statement::DayLight, 0 , 1);
    system.addRule(std::move(rule));

    std::unique_ptr<Rule> rule2 = std::make_unique<Rule>("HEADER", "OFF");
    rule2->addRange(Statement::Temperature, 20 , 90)
         ->addRange(Statement::Weekday, 1 , 1);
    system.addRule(std::move(rule2));

    std::unique_ptr<Rule> rule3 = std::make_unique<Rule>("ALARM", "ON");
    rule3->addRange(Statement::Time, 1000 , 1030)
         ->addRange(Statement::Weekday, 1 , 5);
    system.addRule(std::move(rule3));

    system.saveRules("rules_gen.json");
}

/**
 * @brief The main function of the program.
 */
int main()
{
    ExpertSystem system;

    system.addFact(Statement::Temperature, 20)
          .addFact(Statement::Weekday, 1)
          .addFact(Statement::Time, 1020)
          .addFact(Statement::DayLight, 1)
          .addFact(Statement::ElectricityPrice, 0.1)
          .addFact(Statement::Temperature3h, 20)
          .addFact(Statement::Temperature6h, 20);

    // fill_rules(system);
    system.loadRules("rules.json");
    while (true) {
        interact(system);
        //std::this_thread::sleep_for(std::chrono::minutes(1));
    }

    return 0;
}
