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
    void enter_to_exit(bool display_text = true) {
        Color::SpecStyle specstyle(false, Color::light_black, Color::black, true);
        if (display_text) std::cout << specstyle.get_str() << "Press enter to exit.";

        flush_streams();
        std::cin.get();

        std::cout << Color::SpecStyle().get_str();
    };
}