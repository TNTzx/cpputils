#include <iostream>
#include <fstream>
#include <sstream>

#include "cpputils.hpp"



namespace File {
    std::string read_str_file(const std::string& file_path) {
        std::ifstream file(file_path.c_str());

        std::stringstream str_stream;
        str_stream << file.rdbuf();
        return str_stream.str();
    }
}