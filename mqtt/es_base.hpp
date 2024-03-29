#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @brief Represents different types of statements.
 */
enum class Statement {
    Temperature,        ///< Temperature statement
    Weekday,            ///< Weekday statement
    Time,               ///< Time statement
    Winter,
    Summer,
    Day,
    Night,
    ElPriceHigh,
    ElPriceLow,
    TempHigh,
    TempLow,
    TempLowLow,
    Temp8hLow,
    Temp8hLowLow,
    Temp24hLow,
    Temp24hLowLow,
    Unknown             ///< Unknown statement
};

enum class Weekday {
    Mon,  // Monday
    Tue,  // Tuesday
    Wed,  // Wednesday
    Thu,  // Thursday
    Fri,  // Friday
    Sat,  // Saturday
    Sun   // Sunday
};

struct ExecutorBase {
    virtual void execute(std::string target, std::string action) = 0;
};

/**
 * @brief The Facts class represents a collection of facts and their corresponding values.
 */
class Facts {
private:
    std::map<Statement, bool> facts;

public:
    /**
     * @brief Adds a fact with the specified value to the collection.
     * 
     * @param statement The statement representing the fact.
     * @param value The value associated with the fact.
     */
    void addFact(Statement statement, bool value);

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
    bool getValue(Statement statement);
};
