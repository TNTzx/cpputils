#pragma once



#include <string>
#include <cstddef>
#include <vector>
#include <stdlib.h>
#include <algorithm>

#include "cpputils.hpp"



namespace StrUtils {
    std::string string_repeat(const std::string& str, size_t n) {
        if (n == 0 || str.empty()) return {};
        if (n == 1) return str;
        const auto period = str.size();
        if (period == 1) return std::string(n, str.front());

        std::string ret(str);
        ret.reserve(period * n);
        std::size_t m {2};
        for (; m < n; m *= 2) ret += ret;
        ret.append(ret.c_str(), (n - (m / 2)) * period);
        return ret;
    };


    // Joins a vector of strings using a separator.
    std::string join_strs(std::vector<std::string> vec, std::string sep) {
        std::string out = vec[0];
        for (unsigned int i = 1; i < vec.size(); i++) {
            out += sep + vec[i];
        }
        return out;
    };



    // Checks if the string is a number.
    bool is_number(std::string &str) {
        return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
    };
}