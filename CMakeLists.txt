cmake_minimum_required(VERSION 3.17)
project(chip8_emu LANGUAGES C CXX)

include(cmake/CompilerWarnings.cmake)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(EMULATOR_SOURCES
        ${INC_DIR}/chip8emu/emulator/*.hpp
        ${SRC_DIR}/emulator/*.cpp)

set(DEBUGGER_SOURCES
        ${INC_DIR}/chip8emu/debugger/*.hpp
        ${SRC_DIR}/debugger/*.cpp)

set(CONSOLE_SOURCES
        ${INC_DIR}/chip8emu/console/*.hpp
        ${SRC_DIR}/console/*.cpp)

set(MAIN_SOURCES
        ${INC_DIR}/chip8emu/*.hpp
        ${SRC_DIR}/*.cpp)

file(GLOB SRC_FILES
        ${EMULATOR_SOURCES}
        ${MAIN_SOURCES}
        ${DEBUGGER_SOURCES}
        ${CONSOLE_SOURCES})

file(GLOB IMGUI_SRC
        ${LIB_DIR}/imgui/*.h
        ${LIB_DIR}/imgui/*.cpp)

file(GLOB GLAD_SRC
        ${LIB_DIR}/glad/glad.h
        ${LIB_DIR}/glad/glad.c)

add_executable(chip8_emu ${SRC_FILES} ${IMGUI_SRC} ${GLAD_SRC})
set_project_warnings(chip8_emu)
target_include_directories(chip8_emu PRIVATE ${INC_DIR} ${LIB_DIR})
target_link_libraries(chip8_emu ${SDL2_LIBRARIES} ${CMAKE_DL_LIBS})
