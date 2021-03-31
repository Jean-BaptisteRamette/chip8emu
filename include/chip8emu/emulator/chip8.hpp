#ifndef CHIP8_EMU_CHIP8_HPP
#define CHIP8_EMU_CHIP8_HPP

#include <chip8emu/emulator/device_bus.hpp>
#include <chip8emu/emulator/cartridge.hpp>


namespace emu
{
    class chip8
    {
    public:
        chip8(const std::shared_ptr<SDL_Renderer>& renderer_ptr);
        ~chip8() noexcept = default;
        chip8(const chip8&) = delete;
        chip8(chip8&&) = delete;
        chip8& operator=(const chip8&) = delete;
        chip8& operator=(chip8&&) = delete;

        void insert_cartridge(const cartridge& cartridge);

        [[maybe_unused]] void remove_cartridge();

        /* handle chip8 specific events, such as keypress */
        void handle_physical_event(SDL_Event* event);

        /* execute a simple fetch, decode, execute cycle, and does all the rendering work */
        void execute_cpu_cycle();

        /* communicate with devices */
        device_bus dev_bus;
    };
}

#endif // CHIP8_EMU_CHIP8_HPP