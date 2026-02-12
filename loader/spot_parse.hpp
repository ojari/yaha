#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "../common/utils.hpp"
#include "ICommand.hpp"

using json = nlohmann::json;

struct PriceData {
    int rank;
    std::string dateTime;
    long epoch;
    float priceWithTax;
};

class SpotParse : public ICommand {
public:
    explicit SpotParse(const std::string& cache_file) :
        ICommand("-sp", "Parse spot information from cache file."),
        cache_file(cache_file)
    {}

    virtual ~SpotParse() = default;

    void execute() override {
        const auto prices = parse();
        for (const auto& record : prices) {
            std::cout << "Rank: " << record.rank
                      << ", DateTime: " << record.dateTime
                      << ", PriceWithTax: " << record.priceWithTax
                      << std::endl;
        }
    }

private:
    std::vector<PriceData> parse() const {
        std::ifstream file(cache_file);
        if (!file.is_open()) {
            std::cerr << "ERROR: could not open file: " << cache_file << std::endl;
            return {};
        }

        json payload;
        file >> payload;
        if (payload.is_null()) {
            std::cerr << "ERROR: invalid json in file: " << cache_file << std::endl;
            return {};
        }

        if (!payload.is_array()) {
            std::cerr << "ERROR: expected json array in file: " << cache_file << std::endl;
            return {};
        }

        std::vector<PriceData> prices;
        prices.reserve(payload.size());
        for (const auto& entry : payload) {
            if (!entry.is_object()) {
                continue;
            }

            const int rank = entry.value("Rank", -1);
            const auto date_time = entry.value("DateTime", std::string{});
            const float price_with_tax = entry.value("PriceWithTax", -1.0f);

            if (rank < 0 || date_time.empty() || price_with_tax < 0.0f) {
                continue;
            }

            const auto normalized_date = normalizeDateTime(date_time);
            const long epoch = convert2epoch(normalized_date.c_str());

            prices.push_back(PriceData { rank, normalized_date, epoch, price_with_tax });
        }

        return prices;
    }

    std::string normalizeDateTime(const std::string& value) const {
        const auto t_pos = value.find('T');
        if (t_pos == std::string::npos) {
            return value;
        }

        auto cleaned = value;
        const auto z_pos = cleaned.find('Z', t_pos);
        if (z_pos != std::string::npos) {
            cleaned.resize(z_pos);
            return cleaned;
        }

        auto timezone_pos = cleaned.find('+', t_pos);
        if (timezone_pos == std::string::npos) {
            timezone_pos = cleaned.find('-', t_pos + 1);
        }

        if (timezone_pos != std::string::npos) {
            cleaned.resize(timezone_pos);
        }

        return cleaned;
    }

    std::string cache_file;
};

