#include "expert.hpp"
#include <chrono>
#include <thread>

class MyExecutor : public ExecutorBase {
public:
    MyExecutor() : 
        system()
    {
        system.addFact(ScalarStatement::Temperature, 20)
            .addFact(Statement::Weekday)
            .addFact(ScalarStatement::Time, 1020)
            .addFact(Statement::Day)
            .addFact(Statement::ElPriceHigh)
            .addFact(Statement::Winter)
            .addFact(Statement::TempLowLow)
            .addFact(Statement::TempLow);

        system.loadRules("rules.json");
        //system.saveRules("rules_tmp.json");
    }

    void loop() {
        time + 1;

        system.addFact(ScalarStatement::Time, static_cast<long>(time.getTime()));

        system.infer(*this);
    }

    void execute(std::string target, std::string action) override {
        if (cache.find(target) != cache.end() && cache[target] == action) {
            return;
        }
        cache[target] = action;

        std::cout << std::setw(2) << std::setfill('0') << time.getHour() << ":"
                  << std::setw(2) << std::setfill('0') << time.getMinute() << " "
                  << target << "::" << action << std::endl;
    }
private:
    ExpertSystem system;
    IntTime time {0, 0};
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
    //system.addFact(static_cast<Statement>(fact), value);

    //system.infer();
}   


int main()
{
    MyExecutor executor;

    while (true) {
        executor.loop();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return 0;
}