#include <gtest/gtest.h>
#define private public
#include "../mqtt/task/calc_sun.hpp"
#undef private
#include <string>
#include <array>

// Helper
static int hmToMinutes(const std::string& hhmm) {
    int h = std::stoi(hhmm.substr(0,2));
    int m = std::stoi(hhmm.substr(3,2));
    return h*100 + m;
}

// Authoritative reference (UTC) for Espoo (60.2055 N, 24.6559 E)
// Source: (DOCUMENT HERE, e.g. "NOAA solar calculator downloaded 2025-09-30")
// Values chosen for 2026 (non-leap) � adjust if your algorithm uses another year internally.
// If your algorithm outputs local solar time, convert or adapt expected values.
struct RefRow {
    int year, month, day;
    const char* sunriseUtc; // "HH:MM"
    const char* sunsetUtc;  // "HH:MM"
};

static constexpr std::array<RefRow,5> kRef = {{
    {2026,  1, 15, "08:57", "15:35"},
    {2026,  3, 21, "05:17", "17:28"}, // Around spring equinox (UTC)
    {2026,  6, 21, "01:52", "20:57"}, // Summer solstice
    {2026,  9, 23, "04:19", "16:30"}, // Autumn equinox
    {2026, 12, 21, "09:42", "14:53"}  // Winter solstice
}};

// NOTE: Adjust these times if you verify different authoritative UTC values.
// Always document the generator method.

/*
TEST(TaskCalcSunReferenceTest, MatchesAuthoritativeReferenceWithinTolerance) {
    EventBus bus;
    task::TaskCalcSun task(bus);

    // Decide tolerance (minutes). Start stricter; relax only if justified.
    const int toleranceMinutes = 4;

    for (const auto& row : kRef) {
        //SCOPED_TRACE(::testing::Message()
        //    << row.year << "-" << row.month << "-" << row.day);

        task.sunTime.setLocation(60.2055, 24.6559);
        task.sunTime.setYear(row.year);
        task.sunTime.setDate(row.year, row.month, row.day);

        const double riseRaw = task.sunTime.calculate(true);
        const double setRaw  = task.sunTime.calculate(false);

        auto toMinutes = [](double hoursFrac) {
            const int h = static_cast<int>(hoursFrac);
            const int m = static_cast<int>((hoursFrac - h) * 60.0 + 0.5);
            return h * 60 + m;
        };
        const int riseMin = toMinutes(riseRaw);
        const int setMin  = toMinutes(setRaw);

        const int refRiseMin = hmToMinutes(row.sunriseUtc);
        const int refSetMin  = hmToMinutes(row.sunsetUtc);

        const int diffRise = std::abs(riseMin - refRiseMin);
        const int diffSet  = std::abs(setMin  - refSetMin);

        EXPECT_LE(diffRise, toleranceMinutes);
        EXPECT_LE(diffSet, toleranceMinutes);
    }
}
*/