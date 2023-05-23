#include <iostream>
#include <string>
#include <optional>
#include <functional>

#include "cpputils.hpp"



namespace Console {
    namespace Prompt {
        std::string prompt_raw(std::string prompt) {
            std::string input;
            std::cout << prompt;
            std::getline(std::cin, input);
            return input;
        }


        std::optional<std::string> send_prompt(
            std::string prompt,
            bool is_optional,
            bool show_optional_text,
            std::optional<std::string> blank_input
        ) {
            return send_prompt<std::string>(
                prompt,
                [](std::string input) {return input;},
                is_optional, show_optional_text, blank_input
            );
        }


        int conv_int(std::string input) {
            return stoi(input);
        }




        const char* ExcInputTooLong::what() const noexcept {return "Input is too long.";}
        const char* ExcNotYN::what() const noexcept {return "THe input is not Y or N.";}

        bool send_prompt_yn(
            std::string prompt,
            bool is_optional,
            bool show_optional_text,
            std::optional<std::string> blank_input
        ) {
            // TEST not yn
            // TEST casing
            return send_prompt<bool>(
                prompt + " (Y/N): ",
                [](std::string input) {
                    if (input.size() > 1) throw ExcInputTooLong();

                    char clean_input = tolower((char)input[0]);
                    if (clean_input == 'y') return true;
                    else if (clean_input == 'n') return false;
                    else throw ExcNotYN();
                },
                is_optional, show_optional_text, blank_input
            ).value();
        }


        const char* ExcOutOfRange::what() const noexcept {return "Input is out of range.";}

        std::optional<int> send_prompt_choice(
            std::string prompt,
            std::vector<std::string> choices,
            bool is_optional,
            bool show_optional_text,
            std::optional<std::string> blank_input
        ) {
            std::cout << prompt << std::endl;

            for (int idx = 0; idx < choices.size(); idx++) {
                std::cout << "\t[" << idx + 1 << "]" << choices[idx] << std::endl;
            }

            std::cout << std::endl << std::endl;
            std::optional<int> idx_choice = send_prompt<int>(
                "\t[]: ",
                [choices](std::string input) {
                    int converted = conv_int(input);
                    if (converted > choices.size() || converted < 1) throw ExcOutOfRange();
                    return converted;
                },
                is_optional,
                show_optional_text,
                blank_input
            );

            if (!idx_choice.has_value()) return std::nullopt;
            return idx_choice.value();
        }

    }
}