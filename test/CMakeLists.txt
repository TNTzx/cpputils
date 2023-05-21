cmake_minimum_required(VERSION 3.14)

project(cpputilsTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
include(../cmake/windows-set-path.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(cpputils REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(cpputils_test source/cpputils_test.cpp)
target_link_libraries(cpputils_test PRIVATE cpputils::cpputils)
target_compile_features(cpputils_test PRIVATE cxx_std_17)

add_test(NAME cpputils_test COMMAND cpputils_test)

# ---- End-of-file commands ----

add_folders(Test)
