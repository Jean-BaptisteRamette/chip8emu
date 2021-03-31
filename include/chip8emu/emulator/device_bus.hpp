#ifndef CHIP8_EMU_DEVICE_BUS_HPP
#define CHIP8_EMU_DEVICE_BUS_HPP

#include <chip8emu/emulator/screen_display.hpp>
#include <chip8emu/emulator/audio_unit.hpp>
#include <chip8emu/emulator/processor.hpp>
#include <chip8emu/emulator/keyboard.hpp>
#include <chip8emu/emulator/memory.hpp>


namespace emu
{
    /* Simple device_bus used to communicate between devices */
    struct device_bus final
    {
        explicit device_bus(const std::shared_ptr<SDL_Renderer>& render_ptr) noexcept;
        ~device_bus() noexcept = default;
        device_bus(const device_bus &) = delete;
        device_bus(device_bus &&) = delete;
        device_bus &operator=(const device_bus &) = delete;
        device_bus &operator=(device_bus &&) = delete;

        processor cpu;
        screen_display screen;
        memory ram;
        keyboard keypad;
        audio_unit apu;
    };
}

#endif //CHIP8_EMU_DEVICE_BUS_HPP
