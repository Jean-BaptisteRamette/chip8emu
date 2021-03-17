#ifndef CHIP8_EMU_CHIP8_HPP
#define CHIP8_EMU_CHIP8_HPP

#include <chip8emu/emulator/device_bus.hpp>
#include <chip8emu/emulator/cartridge.hpp>


namespace emulator
{
    class chip8
    {
    public:
        chip8() noexcept = default;
        ~chip8() noexcept = default;
        chip8(const chip8&) = delete;
        chip8(chip8&&) = delete;
        chip8& operator=(const chip8&) = delete;
        chip8& operator=(chip8&&) = delete;

        void insert_cartridge(const cartridge& cartridge);

        [[maybe_unused]] void remove_cartridge();

        void run();

    private:
        /* communicate with devices */
        device_bus m_bus;
    };
}

#endif // CHIP8_EMU_CHIP8_HPP