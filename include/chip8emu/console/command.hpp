#ifndef CHIP8_EMU_COMMAND_HPP
#define CHIP8_EMU_COMMAND_HPP

#include <vector>
#include <string>


namespace csl
{
    struct command
    {
        explicit command(std::string&& cmd);

        const std::string command_line;
        std::string command_name;
        std::vector < std::string > arguments;
    };
}

#endif //CHIP8_EMU_COMMAND_HPP
