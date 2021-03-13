cmake_minimum_required(VERSION 3.17)
project(chip8_emu CXX)

include(cmake/CompilerWarnings.cmake)

set(CMAKE_CXX_STANDARD 20)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SRC_FILES ${INC_DIR}/*.hpp ${SRC_DIR}/*.cpp)

add_executable(chip8_emu ${SRC_FILES})
set_project_warnings(chip8_emu)
target_include_directories(chip8_emu PRIVATE ${INC_DIR})
