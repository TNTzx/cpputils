#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <tuple>

#include "cpputils.hpp"



namespace File {
    std::string read_str_file(std::filesystem::path file_path) {
        std::ifstream file(file_path);

        std::stringstream str_stream;
        str_stream << file.rdbuf();
        return str_stream.str();
    }



    const char* ExcFD::what() const noexcept {return "A file dialog error occurred.";}
    const char* ExcFD_Cancel::what() const noexcept {return "File dialog cancelled.";}


    std::filesystem::path open_file_dialog(const char* title, std::vector<std::tuple<std::string, std::string>> file_types) {
        std::vector<char> file_type_vec;
        for (const auto& file_type_tuple : file_types) {
            const std::string& desc = std::get<0>(file_type_tuple);
            const std::string& file_ext = std::get<1>(file_type_tuple);

            file_type_vec.insert(file_type_vec.end(), desc.begin(), desc.end());
            file_type_vec.push_back('\0');
            file_type_vec.insert(file_type_vec.end(), file_ext.begin(), file_ext.end());
            file_type_vec.push_back('\0');
        }

        // Add the "All Files" filter
        const std::string all_files = "All Files";
        file_type_vec.insert(file_type_vec.end(), all_files.begin(), all_files.end());
        file_type_vec.push_back('\0');
        file_type_vec.push_back('*');
        file_type_vec.push_back('.');
        file_type_vec.push_back('*');
        file_type_vec.push_back('\0');

        std::string file_type_str(file_type_vec.begin(), file_type_vec.end());



        OPENFILENAME ofn;
        char file[MAX_PATH] = "";


        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = file;
        ofn.nMaxFile = sizeof(file);
        ofn.lpstrFilter = file_type_str.c_str();
        ofn.nFilterIndex = 1;
        ofn.lpstrTitle = title;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn)) {
            return file;
        } else {
            DWORD error = CommDlgExtendedError();
            if (error != 0) throw ExcFD();
            else throw ExcFD_Cancel();
        }
    }

    std::filesystem::path get_exe_path() {
        WCHAR path[MAX_PATH];
        GetModuleFileNameW(NULL, path, MAX_PATH);

        return path;
    }
}