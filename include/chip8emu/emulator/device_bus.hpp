#ifndef CHIP8_EMU_DEVICE_BUS_HPP
#define CHIP8_EMU_DEVICE_BUS_HPP

#include <chip8emu/emulator/processor.hpp>
#include <chip8emu/emulator/keyboard.hpp>
#include <chip8emu/emulator/memory.hpp>


namespace emulator
{
    /* Simple device_bus used to communicate between devices */
    struct device_bus final
    {
        device_bus() noexcept;
        ~device_bus() noexcept = default;
        device_bus(const device_bus &) = delete;
        device_bus(device_bus &&) = delete;
        device_bus &operator=(const device_bus &) = delete;
        device_bus &operator=(device_bus &&) = delete;

        processor cpu;
        memory ram;
        keyboard keypad;
    };
}

#endif //CHIP8_EMU_DEVICE_BUS_HPP
