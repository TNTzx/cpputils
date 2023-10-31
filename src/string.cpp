#include <string>
#include <sstream>
#include <cstddef>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <locale>
#include <codecvt>

#include "cpputils.hpp"



namespace StrUtils {
    std::string repeat(const std::string& str, size_t n) {
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
    std::string join(std::vector<std::string> vec, const std::string& sep) {
        std::string out = vec[0];
        for (unsigned int i = 1; i < vec.size(); i++) {
            out += sep + vec[i];
        }
        return out;
    };


    // Splits a string to vectors. Copied from stackoverflow because I am lazy!!!
    std::vector<std::string> split(std::string input, std::string delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = input.find(delimiter, pos_start)) != std::string::npos) {
            token = input.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back(input.substr(pos_start));
        return res;
    }



    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    int unicode_length(std::string input) {
        return converter.from_bytes(input).length();
    }



    // Checks if the string is a number.
    bool is_number(const std::string& str) {
        return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
    };
}