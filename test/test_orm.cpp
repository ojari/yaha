#include <gtest/gtest.h>
#include "../mqtt/data/database.hpp"

auto db = createDatabase("test.db");

TEST(DatabaseTest, InsertsTemperatureRecord) {
    DataTemperature temp { 123, 10.1f, 90.0f };

    db->insert(temp);

    auto temps = db->temperature.begin();

    EXPECT_FLOAT_EQ((*temps).temperature, 10.1f);
    EXPECT_FLOAT_EQ((*temps).humidity, 90.0f);
    EXPECT_EQ((*temps).epoch, 123);

    int counter {0};
    for (auto t : db->temperature) {
        (void)t;
        counter++;
    }
    EXPECT_EQ(counter, 1);
}

TEST(DatabaseWeatherTest, StartsEmpty) {
    auto weather = db->weather.getAll();
    EXPECT_TRUE(weather.empty());
}

TEST(DatabaseWeatherTest, InsertsAndRetrievesWeather) {
    const int TIME = 123;
    const float TEMP = 20.5f;
    const float HUM = 70.0f;
    const float PRESS = 1000.0f;
    const float WINDSPD = 10.0f;
    const float WINDDIR = 180.0f;
    const float RAIN = 1.0f;
    const float UV = 2.2f;
    const float SOLARRAD = 0.4f;
    DataWeather data { TIME, TEMP, HUM, PRESS, WINDSPD, WINDDIR, RAIN, UV, SOLARRAD};

    db->insert(data);

    auto weather = db->weather.getAll();

    ASSERT_EQ(weather.size(), 1);
    EXPECT_EQ(weather[0].epoch, TIME);
    EXPECT_FLOAT_EQ(weather[0].temperature, TEMP);
    EXPECT_FLOAT_EQ(weather[0].humidity, HUM);
    EXPECT_FLOAT_EQ(weather[0].pressure, PRESS);
    EXPECT_FLOAT_EQ(weather[0].windSpeed, WINDSPD);
    EXPECT_FLOAT_EQ(weather[0].windDirection, WINDDIR);
    EXPECT_FLOAT_EQ(weather[0].rain, RAIN);
    EXPECT_FLOAT_EQ(weather[0].uv, UV);
    EXPECT_FLOAT_EQ(weather[0].solarRadiation, SOLARRAD);
}

TEST(DatabaseElPriceTest, StartsEmpty) {
    auto elPrice = db->elPrice.getAll();
    EXPECT_TRUE(elPrice.empty());
}

TEST(DatabaseElPriceTest, InsertsAndRetrievesPrice) {
    const int TIME = 123;
    const float PRICE = 0.15f;
    DataElPrice data { TIME, PRICE };

    db->insert(data);

    auto elPrice = db->elPrice.getAll();

    ASSERT_EQ(elPrice.size(), 1);
    EXPECT_EQ(elPrice[0].epoch, TIME);
    EXPECT_FLOAT_EQ(elPrice[0].price, PRICE);
}

TEST(DatabaseHistoryTest, StartsEmpty) {
    auto history = db->history.getAll();
    EXPECT_TRUE(history.empty());
}

TEST(DatabaseHistoryTest, InsertsAndRetrievesHistoryEntry) {
    const int TIME = 123;
    const std::string DEVICE = "dummy";
    const DataType STATE = DataType::SWITCH;

    DataHistory data { TIME, DEVICE, STATE, 0, 0, 0 };

    db->insert(data);

    auto history = db->history.getAll();

    ASSERT_EQ(history.size(), 1);
    EXPECT_EQ(history[0].epoch, TIME);
    EXPECT_EQ(history[0].type, STATE);
}
