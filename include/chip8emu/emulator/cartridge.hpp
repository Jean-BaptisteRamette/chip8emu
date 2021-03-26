#ifndef CHIP8_EMU_CARTRIDGE_HPP
#define CHIP8_EMU_CARTRIDGE_HPP

#include <chip8emu/inttypes.hpp>
#include <filesystem>
#include <vector>


namespace emu
{
    struct cartridge final
    {
        explicit cartridge(std::filesystem::path rom_path);
        ~cartridge() noexcept = default;

        std::filesystem::path name;
        std::vector<u8> binary;
    };
}

#endif //CHIP8_EMU_CARTRIDGE_HPP
