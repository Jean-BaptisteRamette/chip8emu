#ifndef CHIP8_EMU_PROCESSOR_HPP
#define CHIP8_EMU_PROCESSOR_HPP

#include <stdexcept>
#include <cstdint>
#include <random>
#include <array>
#include <bitset>
#include <chip8emu/inttypes.hpp>


class debugger;

namespace emu
{
    class opcode_decoding_error : public std::runtime_error
    {
    public:
        explicit opcode_decoding_error(u16 opcode)
                : std::runtime_error("Encountered invalid opcode during decoding"),
                  m_opcode(opcode)
        {};

        [[nodiscard]] u16 opcode() const noexcept { return m_opcode; };

    private:
        u16 m_opcode;
    };

    struct device_bus;


    enum proc_flags : u8
    {
        /* as we use std::bitset indexing, these values do not represent a bitmask but a bit position */
        HALT = 0,
        DRAW = 1,
        DECODE_ERROR = 2,
        MEMORY_ERROR = 3
    };

    class processor final
    {
        /* The debugger must access the registers */
        friend debugger;

    public:
        /* PC, I, SP, Timer */
        static constexpr u8 SPECIAL_REG_COUNT { 4 };

        /* V0 - VF */
        static constexpr u8 REG_COUNT  { 16 };
        static constexpr u8 STACK_SIZE { 16 };

    public:
        explicit processor(device_bus &bus) noexcept;
        ~processor() noexcept = default;
        processor(const processor &) = delete;
        processor(processor &&) = delete;
        processor &operator=(const processor &) = delete;
        processor &operator=(processor &&) = delete;

        /* get the next opcode */
        void fetch_instruction() noexcept;

        /* decode opcode then call its corresponding instruction */
        void exec_instruction();

        /* update_frame the delay timer */
        void tick() noexcept;
        [[nodiscard]] u8 timer() const noexcept;

        void flip_flag(proc_flags flag) noexcept;
        void set_flag(proc_flags flag, bool value = true) noexcept;
        [[nodiscard]] bool is_flag_set(proc_flags flag) const noexcept;

        /* CHIP-8 has 34 instructions, they are described here https://en.wikipedia.org/wiki/CHIP-8#Opcode_table ! */
        void invalid_opcode() const;
        void instr00E0();          void instr8XY0() noexcept;
        void instr00EE() noexcept; void instr8XY1() noexcept;
        void instr1NNN() noexcept; void instr8XY2() noexcept;
        void instr2NNN() noexcept; void instr8XY3() noexcept;
        void instr3XKK() noexcept; void instr8XY4() noexcept;
        void instr4XKK() noexcept; void instr8XY5() noexcept;
        void instr5XY0() noexcept; void instr8XY6() noexcept;
        void instr6XKK() noexcept; void instr8XY7() noexcept;
        void instr7XKK() noexcept; void instr8XYE() noexcept;
        void instr9XY0() noexcept; void instrFX0A() noexcept;
        void instrANNN() noexcept; void instrFX07() noexcept;
        void instrBNNN() noexcept; void instrFX1E() noexcept;
        void instrCXKK() noexcept; void instrFX15() noexcept;
        void instrDXYN() noexcept; void instrFX18() noexcept;
        void instrEX9E() noexcept; void instrFX29() noexcept;
        void instrEXA1() noexcept; void instrFX33() noexcept;
        void instrFX55() noexcept; void instrFX65() noexcept;

    private:

        /* link to other devices such as the RAM */
        device_bus& m_bus;

        /* PRNG for CXKK instruction */
        std::mt19937 mt;
        std::uniform_int_distribution<u8> distribution;

        /*!
         * 8-bit general purpose registers
         * Register are called VX, X being a hex digit (0-F)
         * VF is the carry flag register
         */
        std::array<u8, REG_COUNT> V;
        std::array<u16, STACK_SIZE> stack;

        /* special registers */
        u16 PC {};       /* address of the current instruction */
        u16 I  {};       /* special register, stores m_memory addresses */
        u8  SP {};       /* points to the top of the stack */
        u8  DT {255 };   /* delay timer to control CPU clock cycle */

        u16 opcode {};            /* current operation code */
        std::bitset<8> flags {};  /* contains draw flag and halt flag */
    };
}


#endif //CHIP8_EMU_PROCESSOR_HPP
