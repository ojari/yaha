#pragma once

#include <cmath>
#include <cstdint>

namespace automation {

/**
 * @brief Snapshot of contextual inputs provided to the automation rules.
 */
struct Facts {
    enum DirtyFlag : std::uint32_t {
        DirtyNone = 0,
        DirtyTime = 1u << 0,
        DirtyCalendar = 1u << 1,  // weekday, year, month, day
        DirtyOutsideTemp = 1u << 2,
        DirtyStorageTemp = 1u << 3,
        DirtyDownstairTemp = 1u << 4,
        DirtyElectricityPrice = 1u << 5
    };

    /**
     * @brief Clears tracked dirty bits (typically after rules consume the snapshot).
     */
    void clearDirty() noexcept { dirtyFlags = DirtyNone; }

    /**
     * @brief Checks whether specific bits are set.
     */
    [[nodiscard]] bool isDirty(std::uint32_t mask) const noexcept { return (dirtyFlags & mask) != 0u; }

    void setTime(int value) noexcept {
        if (time != value) {
            time = value;
            dirtyFlags |= DirtyTime;
        }
    }

    void setWeekDay(int value) noexcept {
        if (weekDay != value) {
            weekDay = value;
            dirtyFlags |= DirtyCalendar;
        }
    }

    void setDate(int d, int m, int y) noexcept {
        if (day != d) {
            day = d;
            dirtyFlags |= DirtyCalendar;
        }
        if (month != m) {
            month = m;
            dirtyFlags |= DirtyCalendar;
        }
        if (year != y) {
            year = y;
            dirtyFlags |= DirtyCalendar;
        }
    }

    [[nodiscard]] float getOutsideTemp() const noexcept { return outsideTemp; }
    void setOutsideTemp(float value) noexcept {
        if (exceedsResolution(outsideTemp, value, temperatureResolution)) {
            outsideTemp = value;
            dirtyFlags |= DirtyOutsideTemp;
        }
    }

    [[nodiscard]] float getStorageTemp() const noexcept { return storageTemp; }
    void setStorageTemp(float value) noexcept {
        if (exceedsResolution(storageTemp, value, temperatureResolution)) {
            storageTemp = value;
            dirtyFlags |= DirtyStorageTemp;
        }
    }

    [[nodiscard]] float getDownstairTemp() const noexcept { return downstairTemp; }
    void setDownstairTemp(float value) noexcept {
        if (exceedsResolution(downstairTemp, value, temperatureResolution)) {
            downstairTemp = value;
            dirtyFlags |= DirtyDownstairTemp;
        }
    }

    [[nodiscard]] float getElectricityPrice() const noexcept { return electricityPrice; }
    void setElectricityPrice(float value) noexcept {
        if (exceedsResolution(electricityPrice, value, temperatureResolution)) {
            electricityPrice = value;
            dirtyFlags |= DirtyElectricityPrice;
        }
    }

private:
    static constexpr float temperatureResolution = 0.1;

    [[nodiscard]] static bool exceedsResolution(float current, float value, float resolution) noexcept {
        return std::abs(current - value) >= resolution;
    }

    std::uint32_t dirtyFlags = DirtyNone;

    int time = 0;
    int weekDay = 0;
    int year = 0;
    int month = 0;
    int day = 0;
    float outsideTemp = 0.0;
    float storageTemp = 0.0;
    float downstairTemp = 0.0;
    float electricityPrice = 0.0;
};

}  // namespace automation
