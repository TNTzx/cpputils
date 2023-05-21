#pragma once


#include <string>
#include <windows.h>



namespace Console {
    namespace Size {
        CONSOLE_SCREEN_BUFFER_INFO get_csbi() {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            return csbi;
        };


        class ConsoleSize {
            public:
                int columns = 0;
                int rows = 0;

                ConsoleSize(int _columns = 0, int _rows = 0) {
                    this->columns = _columns;
                    this->rows = _rows;
                };
        };

        ConsoleSize get_size() {
            CONSOLE_SCREEN_BUFFER_INFO csbi = get_csbi();
            return ConsoleSize(
                csbi.srWindow.Right - csbi.srWindow.Left + 1,
                csbi.srWindow.Bottom - csbi.srWindow.Top + 1
            );
        };


        ConsoleSize default_size = get_size();


        std::string get_fill_hor(std::string filler, ConsoleSize size = default_size) {
            int filler_size = filler.size();
            std::string new_str = StrUtils::string_repeat(filler, size.columns / filler_size);
            std::string extra = filler.substr(0, size.columns % filler_size);
            return new_str + extra;
        };
    };

}