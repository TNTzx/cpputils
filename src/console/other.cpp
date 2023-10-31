#include <iostream>
#include <windows.h>

#include "cpputils.hpp"



namespace Console {
    // Flushes the cout and cin streams for reuse regardless of errors.
    void flush_streams() {
        std::cout.flush();
        std::cin.sync();
        std::cin.clear();
    }

    // Clears the console. Taken from stackoverflow.com.
    void clear_console() {
        flush_streams();
        std::cout << Console::Color::SpecStyle().get_str();

        COORD tl = {0,0};
        CONSOLE_SCREEN_BUFFER_INFO s;
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(console, &s);
        DWORD written, cells = s.dwSize.X * s.dwSize.Y;
        FillConsoleOutputCharacterW(console, ' ', cells, tl, &written);
        FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
        SetConsoleCursorPosition(console, tl);
    };

    // Prompts the user to press enter to exit.
    void enter_to_exit(bool display_text) {
        Color::SpecStyle specstyle(false, Color::light_black, Color::black, true);
        if (display_text) std::cout << specstyle.get_str() << "Press enter to exit.";

        flush_streams();
        std::cin.ignore();
        std::cin.get();

        std::cout << Color::SpecStyle().get_str();
    };


    // Prints the text indented.
    void print_indented(std::string text, size_t starting_column) {
        size_t current_y = Cursor::get_pos().Y;
        auto splits = StrUtils::split(text, "\n");

        for (size_t idx = 0; idx < splits.size(); idx++) {
            std::string part = splits[idx];

            Cursor::set_pos(starting_column, current_y);
            std::cout << part;

            if (!(idx + 1 == splits.size())) {
                current_y++;
                std::cout << std::endl;
            }
        }
    }


    // Prints the text at the middle of the console horizontally centered.
    void print_horcentered(std::string text) {
        Size::ConsoleSize current_size = Size::get_size();
        int console_center_row = ceil(current_size.columns / 2.0F) - 1;

        auto splits = StrUtils::split(text, "\n");
        int largest_unbroken_length = 0;
        for (std::string part : splits) {
            int length = StrUtils::unicode_length(part);
            if (length > largest_unbroken_length) largest_unbroken_length = length;
        }

        int text_distance_from_center = ceil(largest_unbroken_length / 2.0F) - 1;

        int indent = console_center_row - text_distance_from_center;
        print_indented(text, indent);
    }
}