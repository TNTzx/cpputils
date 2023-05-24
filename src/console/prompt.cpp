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


        const char* Exc_NoInput::what() const noexcept {return "There's no input.";}

        std::optional<std::string> send_prompt(
            std::string prompt,
            bool is_optional,
            bool show_optional_text,
            std::optional<std::string> blank_input
        ) {
            return send_prompt<std::string>(
                prompt,
                [](std::string input) {
                    if (input.size() == 0) throw Exc_NoInput();
                    return input;
                },
                is_optional, show_optional_text, blank_input
            );
        }



        const char* Exc_InvalidInt::what() const noexcept {return "Input is not an integer.";}
        const char* Exc_InputTooBig::what() const noexcept {return "Input is too big.";}

        int conv_int(std::string input) {
            try {
                return stoi(input);
            } catch (const std::invalid_argument& exc) {
                throw Exc_InvalidInt();
            } catch (const std::out_of_range& exc) {
                throw Exc_InputTooBig();
            }
        }




        const char* Exc_InputTooLong::what() const noexcept {return "Input is too long.";}
        const char* Exc_NotYN::what() const noexcept {return "The input is not Y or N.";}

        bool send_prompt_yn(
            std::string prompt,
            bool is_optional,
            bool show_optional_text,
            std::optional<std::string> blank_input
        ) {
            return send_prompt<bool>(
                prompt + " (Y/N): ",
                [](std::string input) {
                    if (input.size() > 1) throw Exc_InputTooLong();

                    char clean_input = tolower((char)input[0]);
                    if (clean_input == 'y') return true;
                    else if (clean_input == 'n') return false;
                    else throw Exc_NotYN();
                },
                is_optional, show_optional_text, blank_input
            ).value();
        }


        const char* Exc_NoChoices::what() const noexcept {return "There are no choices supplied.";}
        const char* Exc_OutOfRange::what() const noexcept {return "Input is out of range.";}

        std::optional<int> send_prompt_choice(
            std::string prompt,
            std::vector<std::string> choices,
            bool is_optional,
            bool show_optional_text,
            std::optional<std::string> blank_input
        ) {
            if (choices.size() == 0) throw Exc_NoChoices();

            std::cout
                << prompt << std::endl
                << "Input the number beside the option you want to pick." << std::endl
                << std::endl;

            for (int idx = 0; idx < choices.size(); idx++) {
                std::cout << "- [" << idx + 1 << "] " << choices[idx] << std::endl;
            }

            std::cout << std::endl << std::endl;
            std::optional<int> idx_choice = send_prompt<int>(
                "[]: ",
                [choices](std::string input) {
                    int converted = conv_int(input);
                    if (converted > choices.size() || converted < 1) throw Exc_OutOfRange();
                    return converted;
                },
                is_optional,
                show_optional_text,
                blank_input
            );

            if (!idx_choice.has_value()) return std::nullopt;
            return idx_choice.value() - 1;
        }

    }
}