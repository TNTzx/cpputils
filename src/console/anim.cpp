#pragma once


#include <iostream>
#include <cmath>

#include "cpputils.hpp"



namespace Console {
    namespace Anim {
        // Represents a console animation.
        class Animator {
            public:
                float duration_s = 1;

                Animator(float _duration_s) {
                    this->duration_s = _duration_s;
                };


                // Runs the animation.
                virtual void run() {};


                // Gets the seconds per frame given frames.
                float get_spf(int frames) {return (float)this->duration_s / (float)frames;};
        };



        class BarHighlight : public Animator {
            public:
                int row = 0;
                int width = 1;
                std::string border = "=";
                Color::SpecStyle style{};

                BarHighlight(
                    float duration,
                    int _row,
                    int _width = 1,
                    std::string _bar = "=",
                    Color::SpecStyle _style = Color::SpecStyle()
                ) : Animator(
                    duration
                ) {
                    this->row = _row;
                    this->width = _width;
                    this->border = _bar;
                    this->style = _style;
                };


                // Draws a line across the screen.
                void draw_line(unsigned int _row, std::string _border) {
                    Size::ConsoleSize size = Size::get_size();

                    std::string fill = Size::get_fill_hor(_border, size);
                    Cursor::PosStore pos_store = Cursor::PosStore::from_current();
                    Cursor::set_pos(0, _row);
                    std::cout << fill;
                    pos_store.restore();
                };

                // Draws two lines with a set amount of space away from a target row.
                void draw_surr_lines(unsigned int _row, unsigned int space, std::string _border) {
                    Size::ConsoleSize size = Size::get_size();

                    int top_row = _row - space;
                    if (!(top_row < 0)) this->draw_line(top_row, _border);

                    int bot_row = (_row + this->width) + space;
                    if (!(bot_row > size.rows)) this->draw_line(bot_row, _border);
                };


                // Draws surrounding blanks.
                void draw_surr_blanks(unsigned int _row, unsigned int space) {
                    this->draw_surr_lines(_row, space, " ");
                };

                // Draws surrounding borders.
                void draw_surr_border(unsigned int _row, unsigned int space) {
                    this->draw_surr_lines(_row, space, this->border);
                };

                // Draws both the blank and border.
                void draw_surr_bundle(unsigned int _row, unsigned int space) {
                    this->draw_surr_border(_row, space);
                    this->draw_surr_blanks(_row, space + 1);
                };


                void run() override {
                    Size::ConsoleSize size = Size::get_size();

                    int top_diff = this->row - 0;
                    int bot_diff = size.rows - (this->row + this->width);

                    int initial_space;
                    if (top_diff == bot_diff) initial_space = top_diff;
                    else if (top_diff > bot_diff) initial_space = top_diff;
                    else initial_space = bot_diff;

                    float spf = this->get_spf(initial_space - 1);

                    std::cout << this->style.get_str();

                    for (; initial_space > 0; initial_space--) {
                        this->draw_surr_bundle(this->row, initial_space);
                        sleep(spf);
                    };
                };
        };



        class WipeScreen : public Animator {
            public:
                Color::SpecStyle style{};
                std::string border = "###";
                bool is_fast = false;

                WipeScreen(
                    float _duration_s,
                    std::string _border = "###",
                    Color::SpecStyle _style = Color::SpecStyle(),
                    bool _is_fast = false
                ) : Animator(
                    _duration_s
                ) {
                    this->is_fast = _is_fast;
                    this->border = _border;
                    this->style = _style;
                };


                // Gets the width of the input line.
                int get_line_width(std::string line) {
                    Size::ConsoleSize size = Size::get_size();
                    float line_length = (float)(line.length());
                    float cons_height = (float)(size.rows);
                    return std::ceil(cons_height / line_length);
                };



                // Draws a part of the line.
                void print_line_part(COORD top_coords, std::string line) {
                    Size::ConsoleSize size = Size::get_size();
                    if (top_coords.X > size.columns) return;

                    Cursor::PosStore pos_store = Cursor::PosStore::from_current();
                    int line_length = line.length();
                    for (int i_line = 0; i_line < line_length; i_line++) {
                        Cursor::set_pos(top_coords.X, top_coords.Y + i_line);
                        std::cout << line[i_line];
                    };
                    pos_store.restore();
                };


                // Draws the entire line.
                void print_line(short int top_column, std::string line) {
                    Size::ConsoleSize size = Size::get_size();
                    int line_length = line.length();

                    short int row = 0;
                    COORD part_coord = {top_column, row};
                    while (true) {
                        this->print_line_part(part_coord, line);

                        part_coord.X--;
                        part_coord.Y += line_length;

                        if (part_coord.Y + line_length > size.rows) break;

                    };

                    int remaining = size.rows - part_coord.Y;
                    if (remaining == 0) return;

                    this->print_line_part(part_coord, line.substr(0, remaining));
                };



                // Draws a blank line.
                void print_blank(short int top_column) {
                    this->print_line(top_column, StrUtils::string_repeat(" ", this->border.length()));
                };

                // Draws a solid line.
                void print_border(short int top_column) {
                    this->print_line(top_column, this->border);
                };


                // Prints both the blank and solid line.
                void print_bundle(short int tl_column) {
                    if (!this->is_fast) {
                        this->print_border(tl_column);
                        this->print_blank(tl_column + 1);
                    } else {
                        this->print_blank(tl_column);
                    }
                }


                void run() override {
                    Size::ConsoleSize size = Size::get_size();
                    int line_width = this->get_line_width(this->border);

                    int start_col = size.columns + line_width + 1;
                    float spf = this->get_spf(start_col);

                    std::cout << this->style.get_str();

                    for (int current_col = start_col; current_col >= -1; current_col -= 1) {
                        this->print_bundle(current_col);
                        sleep(spf);
                    };
                };
        };



        class CornerPixelate : public Animator {
            public:
                Color::SpecStyle style{};

                CornerPixelate(float _duration_s, Color::SpecStyle _style = Color::SpecStyle()) : Animator(_duration_s) {
                    this->style = _style;
                };


                // Draws the diagonal line.
                void draw_line(short int top_column) {
                    Size::ConsoleSize size = Size::get_size();

                    Cursor::PosStore pos_store = Cursor::PosStore::from_current();

                    for (short int row = 0; (top_column < size.columns && row < size.rows); (top_column++, row++)) {
                        if (top_column < 0) continue;
                        Cursor::set_pos(top_column, row);
                        std::cout << ".";
                    };

                    pos_store.restore();
                };


                void run() override {
                    Size::ConsoleSize size = Size::get_size();
                    int column = size.columns - 1;

                    float spf = this->get_spf(size.columns);

                    std::cout << this->style.get_str();

                    for (; column > -(size.rows); column--) {
                        this->draw_line(column);
                        sleep(spf);
                    };
                }
        };
        class Typewriter : public Animator {
            public:
                std::string source;

                Typewriter(float _duration_s, std::string _source) : Animator(_duration_s) {
                    this->source = _source;
                };


                void run() override {
                    int source_length = this->source.length();
                    float spf = this->get_spf(source_length);
                    for (int idx = 0; idx < this->source.length(); idx++) {
                        std::cout << this->source[idx];
                        sleep(spf);
                    };
                };
        };
    };

}