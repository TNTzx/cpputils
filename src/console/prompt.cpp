#pragma once


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
            std::cin >> input;
            return input;
        }

        template <class T_Conv>
        std::optional<T_Conv> send_prompt(
            std::string prompt,
            std::function<T_Conv(std::string)> conv_func,
            bool is_optional = false,
            bool show_optional_text = true,
            std::optional<std::string> blank_input = std::nullopt
        ) {
            if (!blank_input.has_value()) blank_input = "::";

            T_Conv final_input;

            while (true) {
                std::string input = prompt_raw(
                    prompt + (
                        show_optional_text && is_optional
                            ? std::string(" (input \"") + blank_input.value() + "\" to leave blank) "
                            : ""
                    )
                );

                if (is_optional && input == blank_input.value()) {return std::nullopt;}

                try {
                    final_input = conv_func(input);
                } catch (std::exception& exc) {
                    std::cout << "Invalid input. " << exc.what() << std::endl;
                    continue;
                }

                break;
            }

            return final_input;
        }
        std::optional<std::string> send_prompt(
            std::string prompt,
            bool is_optional = false,
            bool show_optional_text = true,
            std::optional<std::string> blank_input = std::nullopt
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




        class ExcInputTooLong : public std::exception {
            public:
                const char* what() const noexcept override {
                    return "Input is too long.";
                }
        };

        class ExcNotYN : public std::exception {
            public:
                const char* what() const noexcept override {
                    return "Input isn't \"Y\" or \"N\".";
                }
        };

        bool send_prompt_yn(
            std::string prompt,
            bool is_optional = false,
            bool show_optional_text = true,
            std::optional<std::string> blank_input = std::nullopt
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

    }
}