#pragma once


#include <iostream>
#include <fstream>
#include <sstream>

#include "cpputils.hpp"


namespace File {
    // Opens a file and returns the contents of the file in string format.
    std::string read_str_file(std::string file_path) {
        std::ifstream file(file_path.c_str());

        std::stringstream str_stream;
        str_stream << file.rdbuf();
        return str_stream.str();
    }
}