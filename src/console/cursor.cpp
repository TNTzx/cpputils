#include <windows.h>

#include "cpputils.hpp"



namespace Console {
    namespace Cursor {
        COORD get_pos() {
            CONSOLE_SCREEN_BUFFER_INFO csbi = Size::get_csbi();
            return csbi.dwCursorPosition;
        };


        void set_pos(short int x, short int y) {
            COORD pos = {x, y};
            HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleCursorPosition(out, pos);
        };
        void set_pos(COORD coords) {
            set_pos(coords.X, coords.Y);
        };


        void set_pos_rel(short int x, short int y) {
            COORD current_pos = get_pos();
            set_pos(current_pos.X + x, current_pos.Y + y);
        };
        void set_pos_rel(COORD coords) {
            set_pos_rel(coords.X, coords.Y);
        };


        PosStore::PosStore() {};
        PosStore::PosStore(COORD _previous_pos) {
            this->previous_pos = _previous_pos;
        };
        PosStore PosStore::from_current() {return PosStore(get_pos());};
        void PosStore::restore() {set_pos(this->previous_pos);};
    };

}