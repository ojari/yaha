#include "catch2/catch_all.hpp"
#include "../mqtt/database.hpp"

auto db = createDatabase("test.db");

TEST_CASE("Test database", "[database]") {
    DataTemperature temp { 123, 10.1, 90.0 };

    db->temperature.insert(temp);

    auto temps = db->temperature.getAll();

    REQUIRE(temps.size() == 1);
    REQUIRE(temps[0].temperature == 10.1f);
    REQUIRE(temps[0].humidity == 90.0f);
    REQUIRE(temps[0].epoch == 123);
}

TEST_CASE("Test Database.weather", "[database]") {
    // auto db = createDatabase("test.db");

    SECTION("Empty weather") {
        auto weather = db->weather.getAll();

        REQUIRE(weather.empty());
    }

    SECTION("Insert and retrieve weather") {
        const int TIME = 123;
        const float TEMP = 20.5;
        const float HUM = 70.0;
        const float PRESS = 1000.0;
        const float WINDSPD = 10.0;
        const float WINDDIR = 180.0;
        const float RAIN = 1.0;
        const float UV = 2.2;
        const float SOLARRAD = 0.4;
        DataWeather data { TIME, TEMP, HUM, PRESS, WINDSPD, WINDDIR, RAIN, UV, SOLARRAD};

        db->weather.insert(data);

        auto weather = db->weather.getAll();

        REQUIRE(weather.size() == 1);
        REQUIRE(weather[0].epoch == TIME);
        REQUIRE(weather[0].temperature == TEMP);
        REQUIRE(weather[0].humidity == HUM);
        REQUIRE(weather[0].pressure == PRESS);
        REQUIRE(weather[0].windSpeed == WINDSPD);
        REQUIRE(weather[0].windDirection == WINDDIR);
        REQUIRE(weather[0].rain == RAIN);
        REQUIRE(weather[0].uv == UV);
        REQUIRE(weather[0].solarRadiation == SOLARRAD);
    }
}

TEST_CASE("Test Database.elPrice", "[database]") {
    // auto db = createDatabase("test.db");

    SECTION("Empty elPrice") {
        auto elPrice = db->elPrice.getAll();

        REQUIRE(elPrice.empty());
    }

    SECTION("Insert and retrieve elPrice") {
        const int TIME = 123;
        const float PRICE = 0.15;
        DataElPrice data { TIME, PRICE };

        db->elPrice.insert(data);

        auto elPrice = db->elPrice.getAll();

        REQUIRE(elPrice.size() == 1);
        REQUIRE(elPrice[0].epoch == TIME);
        REQUIRE(elPrice[0].price == PRICE);
    }
}


TEST_CASE("Test Database.history", "[database]") {
    // auto db = createDatabase("test.db");

    SECTION("Empty history") {
        auto history = db->history.getAll();

        REQUIRE(history.empty());
    }

    SECTION("Insert and retrieve history") {
        const int TIME = 123;
        const std::string DEVICE = "dummy";
        const DataType STATE = DataType::SWITCH;

        DataHistory data { TIME, DEVICE, STATE, 0, 0, 0 };

        db->history.insert(data);

        auto history = db->history.getAll();

        REQUIRE(history.size() == 1);
        REQUIRE(history[0].epoch == TIME);
        // REQUIRE(history[0].device == DEVICE);
        REQUIRE(history[0].type == STATE);
    }
}
