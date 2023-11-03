#include <iostream>
#include <string>
#include <optional>
#include <functional>

#include "cpputils.hpp"



namespace Console {
    namespace Prompt {
        std::string cin_raw() {
            std::string input;
            std::getline(std::cin, input);
            return input;
        }

        std::string prompt_raw(std::string prompt, std::optional<Color::SpecStyle> input_style) {
            input_style = input_style.value_or(Color::SpecStyle(false, Color::black, Color::bold_white, true));

            std::cout << prompt << input_style.value().get_str();
            std::string input = cin_raw();

            std::cout << Color::SpecStyle(true).get_str();

            return input;
        }


        const char* Exc_NoInput::what() const noexcept {return "There's no input.";}

        std::optional<std::string> send_prompt(
            std::string prompt,
            bool is_optional,
            bool show_optional_text,
            std::optional<std::string> blank_input,

            std::optional<Color::SpecStyle> prompt_style,
            std::optional<Color::SpecStyle> input_style,
            std::optional<Color::SpecStyle> error_style,
            std::optional<Color::SpecStyle> optional_style
        ) {
            return send_prompt<std::string>(
                prompt,
                [](std::string input) {
                    if (input.size() == 0) throw Exc_NoInput();
                    return input;
                },
                is_optional, show_optional_text, blank_input,
                prompt_style, input_style, error_style, optional_style
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
            std::optional<std::string> blank_input,

            std::optional<Color::SpecStyle> prompt_style,
            std::optional<Color::SpecStyle> yn_style,
            std::optional<Color::SpecStyle> input_style,
            std::optional<Color::SpecStyle> error_style,
            std::optional<Color::SpecStyle> optional_style
        ) {
            yn_style = yn_style.value_or(Color::SpecStyle(false, Color::black, Color::light_black, true));

            return send_prompt<bool>(
                prompt +
                    " " +
                    yn_style.value().get_str() + "(Y/N):" + 
                    Console::Color::SpecStyle(true).get_str() + " ",

                [](std::string input) {
                    if (input.size() > 1) throw Exc_InputTooLong();

                    char clean_input = tolower((char)input[0]);
                    if (clean_input == 'y') return true;
                    else if (clean_input == 'n') return false;
                    else throw Exc_NotYN();
                },

                is_optional, show_optional_text, blank_input,
                prompt_style, input_style, error_style, optional_style
            ).value();
        }


        const char* Exc_NoChoices::what() const noexcept {return "There are no choices supplied.";}
        const char* Exc_InvalidInputChoice::what() const noexcept {return "Input is not in the list of choices.";}

        std::string send_prompt_choice_raw(
            std::vector<std::string> valid_choices
        ) {
            if (valid_choices.size() == 0) throw Exc_NoChoices();

            std::string input = cin_raw();

            if (std::find(valid_choices.begin(), valid_choices.end(), input) == valid_choices.end()) {
                throw Exc_InvalidInputChoice();
            }

            return input;
        }

        std::optional<int> send_prompt_choice(
            std::string prompt,
            std::vector<std::string> choices,
            bool is_optional,
            bool show_optional_text,
            std::optional<std::string> blank_input,

            std::optional<Color::SpecStyle> prompt_style,
            std::optional<Color::SpecStyle> info_style,
            std::optional<Color::SpecStyle> tag_style,
            std::optional<Color::SpecStyle> input_style,
            std::optional<Color::SpecStyle> error_style,
            std::optional<Color::SpecStyle> optional_style
        ) {
            if (choices.size() == 0) throw Exc_NoChoices();


            prompt_style = prompt_style.value_or(Color::SpecStyle(false, Color::bold_white, Color::black, true));
            info_style = info_style.value_or(Color::SpecStyle(false, Color::light_black, Color::black, true));
            tag_style = tag_style.value_or(Color::SpecStyle(false, Color::white, Color::black, true));

            std::cout
                << prompt_style.value().get_str()
                << prompt << std::endl
                << info_style.value().get_str()
                << "Input the number beside the option you want to pick." << std::endl
                << std::endl;

            for (int idx = 0; idx < choices.size(); idx++) {
                std::cout << tag_style.value().get_str() << "- [" << idx + 1 << "] " << choices[idx] << std::endl;
            }

            std::cout << std::endl << std::endl;
            std::optional<int> idx_choice = send_prompt<int>(
                "[]: ",
                [choices](std::string input) {
                    int converted = conv_int(input);
                    if (converted > choices.size() || converted < 1) throw Exc_InvalidInputChoice();
                    return converted;
                },
                is_optional, show_optional_text, blank_input,
                prompt_style, input_style, error_style, optional_style
            );

            std::cout << std::endl;

            if (!idx_choice.has_value()) return std::nullopt;
            return idx_choice.value() - 1;
        }

    }
}