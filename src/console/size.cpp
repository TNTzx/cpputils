#include <string>
#include <windows.h>

#include "cpputils.hpp"



namespace Console {
    namespace Size {
        CONSOLE_SCREEN_BUFFER_INFO get_csbi() {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            return csbi;
        };


        // ConsoleSize
            ConsoleSize::ConsoleSize(int _columns, int _rows) {
                this->columns = _columns;
                this->rows = _rows;
            };


        ConsoleSize get_size() {
            CONSOLE_SCREEN_BUFFER_INFO csbi = get_csbi();
            return ConsoleSize(
                csbi.srWindow.Right - csbi.srWindow.Left + 1,
                csbi.srWindow.Bottom - csbi.srWindow.Top + 1
            );
        };


        std::string get_fill_hor(std::string filler, ConsoleSize size) {
            int filler_size = filler.size();
            std::string new_str = StrUtils::repeat(filler, size.columns / filler_size);
            std::string extra = filler.substr(0, size.columns % filler_size);
            return new_str + extra;
        };
    };

}