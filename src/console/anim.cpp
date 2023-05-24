#include <iostream>
#include <cmath>

#include "cpputils.hpp"



namespace Console {
    namespace Anim {
        // Represents a console animation.
        Animator::Animator(float _duration_s) {
            this->duration_s = _duration_s;
        };
        float Animator::get_spf(int frames) {return (float)this->duration_s / (float)frames;}



        FillMiddleThenWipe::FillMiddleThenWipe(float _duration_s, Color::SpecStyle _style) : Animator(_duration_s) {
            this->style = _style;
        }

        void FillMiddleThenWipe::draw_vert_line(int column) {
            Console::Size::ConsoleSize size = Console::Size::get_size();

            for (int row = 0; row < size.rows; row++) {
                Console::Cursor::set_pos(column, row);
                std::cout << this->style.get_str() << " ";
            }
        }

        void FillMiddleThenWipe::draw_line_set(int columns_away_from_center) {
            Console::Size::ConsoleSize size = Console::Size::get_size();
            int center = (ceil(float(size.columns) / 2.0)) - 1;

            int left = center - columns_away_from_center;
            if (left >= 0) this->draw_vert_line(left);

            int right = center + columns_away_from_center;
            if (right <= size.columns - 1) this->draw_vert_line(right);
        }

        void FillMiddleThenWipe::anim_mid_line() {
            Console::Size::ConsoleSize size = Console::Size::get_size();
            int center_col = (ceil(float(size.columns) / 2.0)) - 1;

            float spf = this->get_spf(size.rows);
            for (int row = 0; row < size.rows; row++) {
                Console::Cursor::set_pos(center_col, row);
                std::cout << this->style.get_str() << " ";
            }
        }

        void FillMiddleThenWipe::run() {
            Console::Size::ConsoleSize size = Console::Size::get_size();
            float surround_spf = this->get_spf(float(size.columns) / 2.0);

            this->anim_mid_line();

            int columns_away_repeats = floor(float(size.columns) / 2.0);
            for (int columns_away = 1; columns_away <= columns_away_repeats; columns_away++) {
                this->draw_line_set(columns_away);
            }
        }



        BarHighlight::BarHighlight(
            float duration,
            int _row,
            int _width,
            const std::string& _bar,
            Color::SpecStyle _style
        ) : Animator(
            duration
        ) {
            this->row = _row;
            this->width = _width;
            this->border = _bar;
            this->style = _style;
        }

        void BarHighlight::draw_line(unsigned int _row, std::string _border) {
            Size::ConsoleSize size = Size::get_size();

            std::string fill = Size::get_fill_hor(_border, size);
            Cursor::PosStore pos_store = Cursor::PosStore::from_current();
            Cursor::set_pos(0, _row);
            std::cout << fill;
            pos_store.restore();
        };

        void BarHighlight::draw_surr_lines(unsigned int _row, unsigned int space, std::string _border) {
            Size::ConsoleSize size = Size::get_size();

            int top_row = _row - space;
            if (!(top_row < 0)) this->draw_line(top_row, _border);

            int bot_row = (_row + this->width) + space;
            if (!(bot_row > size.rows)) this->draw_line(bot_row, _border);
        };

        void BarHighlight::draw_surr_blanks(unsigned int _row, unsigned int space) {
            this->draw_surr_lines(_row, space, " ");
        };

        void BarHighlight::draw_surr_border(unsigned int _row, unsigned int space) {
            this->draw_surr_lines(_row, space, this->border);
        };

        void BarHighlight::draw_surr_bundle(unsigned int _row, unsigned int space) {
            this->draw_surr_border(_row, space);
            this->draw_surr_blanks(_row, space + 1);
        };


        void BarHighlight::run() {
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



        WipeScreen::WipeScreen(
            float _duration_s,
            const std::string& _border,
            Color::SpecStyle _style,
            bool _is_fast
        ) : Animator(
            _duration_s
        ) {
            this->is_fast = _is_fast;
            this->border = _border;
            this->style = _style;
        };

        int WipeScreen::get_line_width(std::string line) {
            Size::ConsoleSize size = Size::get_size();
            float line_length = (float)(line.length());
            float cons_height = (float)(size.rows);
            return std::ceil(cons_height / line_length);
        };

        void WipeScreen::print_line_part(COORD top_coords, std::string line) {
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

        void WipeScreen::print_line(short int top_column, std::string line) {
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

        void WipeScreen::print_blank(short int top_column) {
            this->print_line(top_column, StrUtils::string_repeat(" ", this->border.length()));
        };

        void WipeScreen::print_border(short int top_column) {
            this->print_line(top_column, this->border);
        };

        void WipeScreen::print_bundle(short int tl_column) {
            if (!this->is_fast) {
                this->print_border(tl_column);
                this->print_blank(tl_column + 1);
            } else {
                this->print_blank(tl_column);
            }
        }

        void WipeScreen::run() {
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



        CornerPixelate::CornerPixelate(float _duration_s, Color::SpecStyle _style) : Animator(_duration_s) {
            this->style = _style;
        };

        void CornerPixelate::draw_line(short int top_column) {
            Size::ConsoleSize size = Size::get_size();

            Cursor::PosStore pos_store = Cursor::PosStore::from_current();

            for (short int row = 0; (top_column < size.columns && row < size.rows); (top_column++, row++)) {
                if (top_column < 0) continue;
                Cursor::set_pos(top_column, row);
                std::cout << ".";
            };

            pos_store.restore();
        };

        void CornerPixelate::run() {
            Size::ConsoleSize size = Size::get_size();
            int column = size.columns - 1;

            float spf = this->get_spf(size.columns);

            std::cout << this->style.get_str();

            for (; column > -(size.rows); column--) {
                this->draw_line(column);
                sleep(spf);
            };
        }



        Typewriter::Typewriter(float _duration_s, std::string _source) : Animator(_duration_s) {
            this->source = _source;
        };

        void Typewriter::run() {
            int source_length = this->source.length();
            float spf = this->get_spf(source_length);
            for (int idx = 0; idx < this->source.length(); idx++) {
                std::cout << this->source[idx];
                sleep(spf);
            };
        };
    };
}