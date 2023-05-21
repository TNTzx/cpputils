#include <iomanip>
#include <iostream>
#include <memory>
#include <ctime>
#include <optional>

#include "cpputils.hpp"



namespace Datetime {
    time_t current_timet = std::time(0);
    std::shared_ptr<time_t> current_timet_ptr = std::make_shared<time_t>(current_timet);


    std::shared_ptr<time_t> tm_to_timet(const std::shared_ptr<std::tm>& tm_dt) {
        return std::make_shared<time_t>(std::mktime(tm_dt.get()));
    }

    std::shared_ptr<std::tm> timet_to_tm(const std::shared_ptr<time_t>& time_dt) {
        return std::make_shared<std::tm>(*(std::localtime(time_dt.get())));
    }


    std::shared_ptr<std::tm> current_tm = timet_to_tm(current_timet_ptr);


    std::string date_format(const std::shared_ptr<std::tm>& tm_dt) {
        std::stringstream ss;
        ss << std::put_time(tm_dt.get(), "%b. %d, %Y");
        return ss.str();
    }
    std::string date_format(const std::shared_ptr<time_t>& time_dt) {
        return date_format(timet_to_tm(time_dt));
    }


    std::shared_ptr<time_t> ask_date(std::string prompt, std::string prefix = "-\t", std::string blank_input = "::") {
        std::tm tm_dt;

        std::cout << std::endl << prompt << std::endl;
        std::cout << (prefix + "Input \"" + blank_input + "\" to set as current time.") << std::endl;


        std::optional<int> tm_mday = Console::Prompt::send_prompt<int>(
            prefix + "Enter day of the month: ",
            Console::Prompt::conv_int,
            true
        );
        if (tm_mday.has_value()) tm_dt.tm_mday = tm_mday.value(); else tm_dt.tm_mday = current_tm->tm_mday;

        std::optional<int> tm_mon = Console::Prompt::send_prompt<int>(
            prefix + "Enter month number (1 = January, 2 = February, ..., 12 = December): ",
            Console::Prompt::conv_int,
            true
        );
        if (tm_mon.has_value()) tm_dt.tm_mon = tm_mon.value() - 1; else tm_dt.tm_mon = current_tm->tm_mon;


        std::optional<int> tm_year = Console::Prompt::send_prompt<int>(prefix + "Enter year: ", Console::Prompt::conv_int, true);
        if (tm_year.has_value()) tm_dt.tm_year = tm_year.value() - 1; else tm_dt.tm_year = current_tm->tm_year;

        std::cout << std::endl;

        return tm_to_timet(std::make_shared<std::tm>(tm_dt));
    }
}