#include <chip8emu/console/command.hpp>

namespace csl
{
    command::command(std::string&& cmd) : command_line(std::move(cmd))
    {
        /* parse command name and argument */
    }
}
