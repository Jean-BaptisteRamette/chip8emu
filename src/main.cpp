#include <iostream>
#include <chip8emu/emulator/chip8.hpp>


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "[ERROR]: No ROM input file provided !" << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        emulator::chip8 emu;
        emulator::cartridge cartridge(argv[1]);

        emu.insert_cartridge(cartridge);
        emu.run();

    } catch (const std::runtime_error& err)
    {
        std::cerr << "[ERROR]: " << err.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
