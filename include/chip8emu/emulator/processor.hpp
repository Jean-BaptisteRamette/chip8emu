#ifndef CHIP8_EMU_PROCESSOR_HPP
#define CHIP8_EMU_PROCESSOR_HPP

#include <cstdint>
#include <array>
#include <chip8emu/inttypes.hpp>


namespace emulator
{
    class opcode_decoding_error;

    struct device_bus;

    class processor final
    {
    public:
        static constexpr u8 REG_COUNT  { 16 };
        static constexpr u8 STACK_SIZE { 16 };

    public:
        explicit processor(device_bus &bus) noexcept;
        ~processor() noexcept = default;
        processor(const processor &) = delete;
        processor(processor &&) = delete;
        processor &operator=(const processor &) = delete;
        processor &operator=(processor &&) = delete;

    private:
        /* link to other devices such as the RAM */
        device_bus& m_bus;

        /*!
         * @brief 8-bit general purpose registers
         * Register are called VX, X being a hex digit (0-F)
         * VF is the carry flag register
         */
        std::array<u8, REG_COUNT> V;
        std::array<u16, STACK_SIZE> stack;

        /* special registers */
        u16 PC;      /* address of the current instruction */
        u16 I  {};   /* special register, stores memory addresses */
        u8  SP {};   /* points to the top of the stack */

        u16 opcode {};        /* current operation code */
        u8  delay_timer {};   /* timer to control CPU clock cycle */
    };
}


#endif //CHIP8_EMU_PROCESSOR_HPP
