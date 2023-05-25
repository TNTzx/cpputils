#pragma once


#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>
#include <ctime>
#include <optional>
#include <functional>
#include <string>
#include <exception>
#include <filesystem>
#include <vector>
#include <tuple>



namespace Datetime {
    extern time_t current_timet;
    extern std::tm current_tm;

    time_t tm_to_timet(std::tm tm_dt);
    std::tm timet_to_tm(time_t time_dt);

    std::string date_format(std::tm tm_dt);
    std::string date_format(time_t time_dt);

    time_t ask_date(const std::string& prompt, const std::string& prefix = std::string("-\t"), const std::string& blank_input = std::string("::"));
}



namespace File {
    std::string read_str_file(std::filesystem::path file_path);


    class ExcFD : public std::exception {
        public:
            const char* what() const noexcept override;
    };
    class ExcFD_Cancel : public ExcFD {
        public:
            const char* what() const noexcept override;
    };

    std::filesystem::path open_file_dialog(const char* title = "Select a file...", std::vector<std::tuple<std::string, std::string>> file_types = {});

    std::filesystem::path get_exe_path();
}



void sleep(float seconds);



namespace StrUtils {
    // Repeats the string a set amount of times. Taken from stackoverflow.com for the most efficient method.
    std::string string_repeat(const std::string& str, size_t n);

    // Joins a vector of strings using a separator.
    std::string join_strs(std::vector<std::string> vec, const std::string& sep);

    // Checks if the string is a number.
    bool is_number(const std::string& str);
}



namespace Console {
    namespace Color {
        enum ColorValue {
            black, red, green, yellow, blue, purple, aqua, white,
            light_black, light_red, light_green, light_yellow, light_blue, light_purple, light_aqua, bold_white
        };


        extern const std::string color_values_general[];
        inline std::string get_color_general(ColorValue console_color);


        class GenStyle {
            public:
                ColorValue text_color;
                ColorValue bg_color;

                GenStyle(ColorValue _text_color = ColorValue::white, ColorValue _bg_color = ColorValue::black);

                void set_console_color();
        };


        extern const std::string color_val_spec_text[];
        inline std::string get_color_spec_text(ColorValue console_color);

        extern const std::string color_val_spec_bg[];
        inline std::string get_color_spec_bg(ColorValue console_color);

        enum TextStyle {reset, bold, italic, underline};
        extern const std::string text_style_str[];
        inline std::string get_text_style(TextStyle text_style);


        extern const std::string specstyle_header;
        extern const std::string specstyle_footer;
        class SpecStyle {
            public:
                ColorValue text_color = ColorValue::bold_white;
                ColorValue bg_color = ColorValue::light_black;

                bool bold = false;
                bool italic = false;
                bool underline = false;
                bool reset = true;

                SpecStyle(
                    bool _reset = true,
                    ColorValue _text_color = ColorValue::bold_white,
                    ColorValue _bg_color = ColorValue::black,
                    bool _bold = false,
                    bool _italic = false,
                    bool _underline = false
                );
                SpecStyle(
                    bool _reset,
                    int _text_color,
                    int _bg_color,
                    bool _bold = false,
                    bool _italic = false,
                    bool _underline = false
                );

                static SpecStyle from_genstyle(GenStyle &gen_style);

                std::string get_str();
        };
    }



    namespace Anim {
        // Represents a console animation.
        class Animator {
            public:
                float duration_s = 1;

                Animator(float _duration_s);

                // Runs the animation.
                virtual void run() {};

                // Gets the seconds per frame given frames.
                float get_spf(int frames);
        };



        class BarHighlight : public Animator {
            public:
                int row = 0;
                int width = 1;
                std::string border = std::string("=");
                Console::Color::SpecStyle style;

                BarHighlight(
                    float duration,
                    int _row,
                    int _width = 1,
                    const std::string& _bar = "=",
                    Color::SpecStyle _style = Color::SpecStyle()
                );


                // Draws a line across the screen.
                void draw_line(unsigned int _row, std::string _border);

                // Draws two lines with a set amount of space away from a target row.
                void draw_surr_lines(unsigned int _row, unsigned int space, std::string _border);

                // Draws surrounding blanks.
                void draw_surr_blanks(unsigned int _row, unsigned int space);

                // Draws surrounding borders.
                void draw_surr_border(unsigned int _row, unsigned int space);

                // Draws both the blank and border.
                void draw_surr_bundle(unsigned int _row, unsigned int space);

                void run() override;
        };



        class WipeScreen : public Animator {
            public:
                Color::SpecStyle style{};
                std::string border = "###";
                bool is_fast = false;

                WipeScreen(
                    float _duration_s,
                    const std::string& _border = std::string("###"),
                    Color::SpecStyle _style = Color::SpecStyle(),
                    bool _is_fast = false
                );

                // Gets the width of the input line.
                int get_line_width(std::string line);

                // Draws a part of the line.
                void print_line_part(COORD top_coords, std::string line);

                // Draws the entire line.
                void print_line(short int top_column, std::string line);

                // Draws a blank line.
                void print_blank(short int top_column);

                // Draws a solid line.
                void print_border(short int top_column);

                // Prints both the blank and solid line.
                void print_bundle(short int tl_column);

                void run() override;
        };



        class FillMiddleThenWipe : public Animator {
            public:
                Color::SpecStyle style{};

                FillMiddleThenWipe(float _duration_s, Color::SpecStyle _style = Color::SpecStyle(false, Color::green, Color::green));


                // Draws the vertical line.
                void draw_vert_line(int column);

                // Draws the set of vertical lines.
                void draw_line_set(int columns_away_from_center);

                // Animates drawing the middle line.
                void anim_mid_line();


                void run() override;
        };



        class CornerPixelate : public Animator {
            public:
                Color::SpecStyle style{};

                CornerPixelate(float _duration_s, Color::SpecStyle _style = Color::SpecStyle());


                // Draws the diagonal line.
                void draw_line(short int top_column);


                void run() override;
        };



        class Typewriter : public Animator {
            public:
                std::string source;

                Typewriter(float _duration_s, std::string _source);

                void run() override;
        };
    }



    namespace Cursor {
        COORD get_pos();

        void set_pos(short int x, short int y);
        void set_pos(COORD coords);

        void set_pos_rel(short int x, short int y);
        void set_pos_rel(COORD coords);


        class PosStore {
            public:
                COORD previous_pos;

                PosStore();
                PosStore(COORD _previous_pos);


                static PosStore from_current();

                void restore();
        };
    }



    namespace Prompt {
        std::string prompt_raw(
            std::string prompt,
            std::optional<Color::SpecStyle> input_style = std::nullopt
        );



        class ExcP_PromptError : public std::exception {};



        class Exc_NoInput : public ExcP_PromptError {
            public:
                const char* what() const noexcept override;
        };



        template <class T_Conv>
        std::optional<T_Conv> send_prompt(
            std::string prompt,
            std::function<T_Conv(std::string)> conv_func,
            bool is_optional = false,
            bool show_optional_text = true,
            std::optional<std::string> blank_input = std::nullopt,

            std::optional<Color::SpecStyle> prompt_style = std::nullopt,
            std::optional<Color::SpecStyle> input_style = std::nullopt,
            std::optional<Color::SpecStyle> error_style = std::nullopt,
            std::optional<Color::SpecStyle> optional_style = std::nullopt
        ) {
            blank_input = blank_input.value_or("::");

            prompt_style = prompt_style.value_or(Color::SpecStyle(false, Color::bold_white, Color::black, true));
            error_style = error_style.value_or(Color::SpecStyle(false, Color::light_red, Color::black, true));
            optional_style = optional_style.value_or(Color::SpecStyle(false, Color::light_black, Color::black, false, true));


            T_Conv final_input;

            while (true) {
                std::string input = prompt_raw(
                    prompt_style.value().get_str() +
                    prompt + (
                        show_optional_text && is_optional
                            ? optional_style.value().get_str() + " (input \"" + blank_input.value() + "\" to leave blank) " + Color::SpecStyle(true).get_str()
                            : ""
                    ),
                    input_style
                );

                if (is_optional && input == blank_input.value()) {return std::nullopt;}

                try {
                    final_input = conv_func(input);
                } catch (std::exception& exc) {
                    std::cout << error_style.value().get_str() << "Invalid input. " << exc.what() << Color::SpecStyle(true).get_str() << std::endl;
                    continue;
                }

                break;
            }

            return final_input;
        };
        std::optional<std::string> send_prompt(
            std::string prompt,
            bool is_optional = false,
            bool show_optional_text = true,
            std::optional<std::string> blank_input = std::nullopt,

            std::optional<Color::SpecStyle> prompt_style = std::nullopt,
            std::optional<Color::SpecStyle> input_style = std::nullopt,
            std::optional<Color::SpecStyle> error_style = std::nullopt,
            std::optional<Color::SpecStyle> optional_style = std::nullopt
        );


        class Exc_InvalidInt : public ExcP_PromptError {
            public:
                const char* what() const noexcept override;
        };
        class Exc_InputTooBig : public ExcP_PromptError {
            public:
                const char* what() const noexcept override;
        };

        int conv_int(std::string input);



        class Exc_InputTooLong : public ExcP_PromptError {
            public:
                const char* what() const noexcept override;
        };

        class Exc_NotYN : public ExcP_PromptError {
            public:
                const char* what() const noexcept override;
        };

        bool send_prompt_yn(
            std::string prompt,
            bool is_optional = false,
            bool show_optional_text = true,
            std::optional<std::string> blank_input = std::nullopt,

            std::optional<Color::SpecStyle> prompt_style = std::nullopt,
            std::optional<Color::SpecStyle> yn_style = std::nullopt,
            std::optional<Color::SpecStyle> input_style = std::nullopt,
            std::optional<Color::SpecStyle> error_style = std::nullopt,
            std::optional<Color::SpecStyle> optional_style = std::nullopt
        );



        class Exc_NoChoices : public ExcP_PromptError {
            public:
                const char* what() const noexcept override;
        };
        class Exc_OutOfRange : public ExcP_PromptError {
            public:
                const char* what() const noexcept override;
        };

        std::optional<int> send_prompt_choice(
            std::string prompt,
            std::vector<std::string> choices,
            bool is_optional = false,
            bool show_optional_text = true,
            std::optional<std::string> blank_input = std::nullopt,

            std::optional<Color::SpecStyle> prompt_style = std::nullopt,
            std::optional<Color::SpecStyle> info_style = std::nullopt,
            std::optional<Color::SpecStyle> tag_style = std::nullopt,
            std::optional<Color::SpecStyle> input_style = std::nullopt,
            std::optional<Color::SpecStyle> error_style = std::nullopt,
            std::optional<Color::SpecStyle> optional_style = std::nullopt
        );
    }



    namespace Size {
        CONSOLE_SCREEN_BUFFER_INFO get_csbi();


        class ConsoleSize {
            public:
                int columns = 0;
                int rows = 0;

                ConsoleSize(int _columns = 0, int _rows = 0);
        };


        ConsoleSize get_size();

        std::string get_fill_hor(std::string filler, ConsoleSize size = get_size());
    }




    // Flushes the cout and cin streams for reuse regardless of errors.
    void flush_streams();

    // Clears the console. Taken from stackoverflow.com.
    void clear_console();

    // Prompts the user to press enter to exit.
    void enter_to_exit(bool display_text = true);
}
