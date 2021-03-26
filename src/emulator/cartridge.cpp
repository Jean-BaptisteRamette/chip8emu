#include <chip8emu/emulator/cartridge.hpp>
#include <chip8emu/emulator/memory.hpp>
#include <fstream>


namespace emu
{
    cartridge::cartridge(std::filesystem::path rom_path)
        : name(std::move(rom_path))
    {
        /* open file */
        std::ifstream rom(name, std::ios_base::binary | std::ios_base::in);

        if (!rom)
            throw std::runtime_error("Could not open rom: " + name.string());

        /* make sure ROM fits in memory's program range */
        const auto rom_size { std::filesystem::file_size(name) };

        if (rom_size > memory::CODE_MAXSIZE)
        {
            const std::string error { "Could not load machine code because program size must not exceed "
                    + std::to_string(memory::CODE_MAXSIZE) + "o.\n"
                    + name.string() + "'s size: " + std::to_string(rom_size) + "o."
            };

            throw std::runtime_error(error);
        }

        binary.resize(rom_size);

        /* read machine code */
        char* buffer { reinterpret_cast<char*>(std::data(binary)) };
        if (!rom.read(buffer, static_cast<std::streamsize>(rom_size)))
            throw std::runtime_error("Could not read rom: " + name.string());
    }
}
