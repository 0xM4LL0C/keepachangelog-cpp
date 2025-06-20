cmake_minimum_required(VERSION 3.10)

project(keepachangelog LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(cmake/CPM.cmake)

CPMAddPackage("gh:Neargye/semver@1.0.0-rc")
CPMAddPackage("gh:HowardHinnant/date@3.0.4")

add_library(keepachangelog STATIC src/keepachangelog.cpp)

target_link_libraries(
    keepachangelog
    PUBLIC semver::semver
    PUBLIC date::date
)

target_include_directories(keepachangelog PUBLIC include)
