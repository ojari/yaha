#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "automation.hpp"
#include "facts.hpp"

namespace automation {

class Lights : public Automation {
public:
    struct Condition {
        enum class Group {
            Time,
            Dark,
            ElectricityPrice
        };

        explicit Condition() = default;
        virtual ~Condition() = default;
    
        [[nodiscard]] virtual bool matches(const Facts& facts) const = 0;
        [[nodiscard]] virtual Group group() const = 0;
    };

    struct TimeCondition : Condition {
        TimeCondition(int onTime, int offTime);
        [[nodiscard]] bool matches(const Facts& facts) const override;
        [[nodiscard]] Group group() const override { return Group::Time; }

    private:
        int onTime;
        int offTime;
    };

    struct DarkCondition : Condition {
        DarkCondition(bool whenDark);
        [[nodiscard]] bool matches(const Facts& facts) const override;
        [[nodiscard]] Group group() const override { return Group::Dark; }

    private:
        bool whenDark;
    };

    struct ElectricityPriceCondition : Condition {
        ElectricityPriceCondition(double threshold, bool above);
        [[nodiscard]] bool matches(const Facts& facts) const override;
        [[nodiscard]] Group group() const override { return Group::ElectricityPrice; }

    private:
        double threshold;
        bool above;
    };

    Lights(std::shared_ptr<IAutomationOutput> output, const std::string& name);

    void setCondition(const std::string &type, const nlohmann::json& data);
    void setArg(const std::string& name, const std::string& value) override;

    void registerEvents(EventBus& evbus) override;

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IAutomationOutput> output)
    {
        return std::make_shared<Lights>(output, name);
    }
    void onEvent(const TimeEvent& event);
    void onEvent(const DateEvent& event);
    void onEvent(const DarkEvent& event);
    void onEvent(const ElectricityPriceEvent& event);

private:

    void updateState();

    Facts facts;
    std::vector<std::unique_ptr<Condition>> conditions;
    int brightness = NAN_VALUE;
};

}  // namespace automation
