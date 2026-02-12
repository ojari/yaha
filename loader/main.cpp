#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
//#include <sstream>
//#include <fstream>
#include <algorithm>
#include <array>
#include "../common/utils.hpp"
#include "spot_download.hpp"
#include "spot_parse.hpp"
#include "weather_download.hpp"
#include "weather_parse.hpp"


int main(int argc, char** argv)
{
    const std::string cache_spot { "tmp_stock.json" };
    const std::string cache_weather { "tmp_weather.xml" };
    const std::string flat_file { "prices.bin" };

    const std::array<std::unique_ptr<ICommand>, 4> commands {
        std::make_unique<SpotDownload>(cache_spot),
        std::make_unique<SpotParse>(cache_spot),
 		std::make_unique<WeatherDownload>(cache_weather),
		std::make_unique<WeatherParse>(cache_weather)
    };

    if (argc == 2) {
        std::string arg = argv[1];
        for (const auto& command : commands) {
            if (command->flag() == arg) {
                command->execute();
                break;
            }
        }
    } else {
        std::cout << "Command line argument missing!" << std::endl;
        std::cout << std::endl;
        std::cout << "Commands:" << std::endl;
        for (const auto& command : commands) {
            std::cout << "  " << command->flag() << " - " << command->description() << std::endl;
        }
    }
    return 0;
}
