#include "expert.hpp"
#include <iostream>
#include <map>
#include <chrono>
#include <thread>
#include <iomanip>

class MyExecutor : public ExecutorBase {
public:
    void setTime(int time) {
        this->time = time;
    }
    void execute(std::string target, std::string action) override {
        if (cache.find(target) != cache.end() && cache[target] == action) {
            return;
        }
        cache[target] = action;

        int hour = time / 100;
        int minute = time % 100;

        std::cout << std::setw(2) << std::setfill('0') << hour << ":"
                  << std::setw(2) << std::setfill('0') << minute << " "
                  << target << "::" << action << std::endl;
    }
private:
    int time = 0;
    std::map<std::string, std::string> cache;
};

/**
 * @brief Interacts with the expert system by adding facts and getting the inferred action.
 */
void interact(ExpertSystem& system)
{
    int fact;
    int value;
    std::cout << "Enter a fact:" << std::endl;
    print_statements();

    std::cin >> fact;
    std::cout << "Enter a value: ";
    std::cin >> value;
    std::cin.ignore();  // ignore the newline character
    system.addFact(static_cast<Statement>(fact), value);

    //system.infer();
}   

/**
 * @brief The main function of the program.
 */
int main()
{
    MyExecutor executor;
    ExpertSystem system;

    system.addFact(Statement::Temperature, 20)
          .addFact(Statement::Weekday, 1)
          .addFact(Statement::Time, 1020)
          .addFact(Statement::Day, 1)
          .addFact(Statement::ElPriceHigh, 1)
          .addFact(Statement::Winter, 1)
          .addFact(Statement::TempLowLow, 1)
          .addFact(Statement::TempLow, 1);

    system.loadRules("rules.json");
    //system.saveRules("rules_tmp.json");

    for(int hour = 0; hour<24; hour++){
        for (int minute = 0; minute < 60; minute++) {
            long time = hour*100 + minute;

            executor.setTime(time);
            system.addFact(Statement::Time, time);
            system.infer(executor);

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
    return 0;
}
