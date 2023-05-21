cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    cpputils
    VERSION 0.1.0
    DESCRIPTION "wawa"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES NONE
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(cpputils_cpputils INTERFACE)
add_library(cpputils::cpputils ALIAS cpputils_cpputils)

set_property(
    TARGET cpputils_cpputils PROPERTY
    EXPORT_NAME cpputils
)

target_include_directories(
    cpputils_cpputils ${warning_guard}
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(cpputils_cpputils INTERFACE cxx_std_17)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT cpputils_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of cpputils"
  )
endif()

include(cmake/dev-mode.cmake)
