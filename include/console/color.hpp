#pragma once


#include <iostream>

#include "../string.hpp"



namespace Console {
    namespace Color {
        enum ColorValue {
            black, red, green, yellow, blue, purple, aqua, white,
            light_black, light_red, light_green, light_yellow, light_blue, light_purple, light_aqua, bold_white
        };


        const std::string color_values_general[] = {
            "0", "1", "2", "3", "4", "5", "6", "7",
            "8", "9", "A", "B", "C", "D", "E", "F"
        };
        inline std::string get_color_general(ColorValue console_color) {
            return color_values_general[console_color];
        };


        class GenStyle {
            public:
                ColorValue text_color;
                ColorValue bg_color;

                GenStyle(ColorValue _text_color = ColorValue::white, ColorValue _bg_color = ColorValue::black) {
                    this->bg_color = _bg_color;
                    this->text_color = _text_color;
                };


                void set_console_color() {
                    std::string cmd = "color " + get_color_general(this->bg_color) + get_color_general(this->text_color);
                    system(cmd.c_str());
                };
        };


        const std::string color_val_spec_text[] = {
            "30", "31", "32", "33", "34", "35", "36", "37",
            "90", "91", "92", "93", "94", "95", "96", "97"
        };
        inline std::string get_color_spec_text(ColorValue console_color) {
            return color_val_spec_text[console_color];
        };

        const std::string color_val_spec_bg[] = {
            "40", "41", "42", "43", "44", "45", "46", "47",
            "100", "101", "102", "103", "104", "105", "106", "107"
        };
        inline std::string get_color_spec_bg(ColorValue console_color) {
            return color_val_spec_bg[console_color];
        };



        enum TextStyle {reset, bold, italic, underline};
        const std::string text_style_str[] = {"0", "1", "3", "4"};
        inline std::string get_text_style(TextStyle text_style) {
            return text_style_str[text_style];
        };


        std::string specstyle_header = "\u001b[";
        std::string specstyle_footer = "m";
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
                ) {
                    this->text_color = _text_color;
                    this->bg_color = _bg_color;
                    this->bold = _bold;
                    this->italic = _italic;
                    this->underline = _underline;
                    this->reset = _reset;
                };
                SpecStyle(
                    bool _reset,
                    int _text_color,
                    int _bg_color,
                    bool _bold = false,
                    bool _italic = false,
                    bool _underline = false
                ) {
                    this->text_color = static_cast<ColorValue>(_text_color);
                    this->bg_color = static_cast<ColorValue>(_bg_color);
                    this->bold = _bold;
                    this->italic = _italic;
                    this->underline = _underline;
                    this->reset = _reset;
                };
                static SpecStyle from_genstyle(GenStyle &gen_style) {
                    return SpecStyle(false, gen_style.text_color, gen_style.bg_color);
                };


                std::string get_str() {
                    if (this->reset) {
                        return specstyle_header + "0" + specstyle_footer;
                    };

                    std::vector<std::string> textstyles;

                    textstyles.push_back(get_color_spec_text(this->text_color));
                    textstyles.push_back(get_color_spec_bg(this->bg_color));

                    if (this->bold) {
                        textstyles.push_back(get_text_style(TextStyle::bold));
                    };
                    if (this->italic) {
                        textstyles.push_back(get_text_style(TextStyle::italic));
                    };
                    if (this->underline) {
                        textstyles.push_back(get_text_style(TextStyle::underline));
                    };
                    return specstyle_header + StrUtils::join_strs(textstyles, ";") + specstyle_footer;
                }
        };
    };

}