#include <chip8emu/emulator/device_bus.hpp>
#include <chip8emu/emulator/processor.hpp>
#include <chip8emu/emulator/memory.hpp>

namespace emulator
{
#define OPCODE_GETX(opcode) (opcode & 0x0F00u) >> 8u  // OPCODE_GETX(0x8F13) -> 0xF
#define OPCODE_GETY(opcode) (opcode & 0x00F0u) >> 4u  // OPCODE_GETY(0x8F13) -> 0x1
#define JMP_NEXT(pc) pc += 2  // jumps to the next instruction

processor::processor(device_bus &bus) noexcept
        : m_bus{bus}, V{}, stack{}, PC(memory::CODE_ADDRESS)
{}

void processor::fetch_instruction() noexcept
{
    opcode = static_cast<u16>((m_bus.ram[PC] << 8u) | m_bus.ram[PC + 1]);
    JMP_NEXT(PC);
}

void processor::tick() noexcept
{
    --delay_timer;
}

[[nodiscard]] u8 processor::timer() const noexcept
{
    return delay_timer;
}

void processor::exec_instruction()
{
    switch ((opcode & 0xF000u) >> 0xCu)
    {
        case 0x0:
            switch (opcode & 0x000Fu)
            {
                case 0x0: instr00E0(); break;
                case 0xE: instr00EE(); break;
                default: invalid_opcode();
            }
            break;

        case 0x1: instr1NNN(); break;
        case 0x2: instr2NNN(); break;
        case 0x3: instr3XKK(); break;
        case 0x4: instr4XKK(); break;
        case 0x5: instr5XY0(); break;
        case 0x6: instr6XKK(); break;
        case 0x7: instr7XKK(); break;
        case 0x9: instr9XY0(); break;
        case 0xA: instrANNN(); break;
        case 0xB: instrBNNN(); break;
        case 0xC: instrCXKK(); break;
        case 0xD: instrDXYN(); break;

        case 8:
            switch (opcode & 0x000Fu)
            {
                case 0x0: instr8XY0(); break;
                case 0x1: instr8XY1(); break;
                case 0x2: instr8XY2(); break;
                case 0x3: instr8XY3(); break;
                case 0x4: instr8XY4(); break;
                case 0x5: instr8XY5(); break;
                case 0x6: instr8XY6(); break;
                case 0x7: instr8XY7(); break;
                case 0xE: instr8XYE(); break;
                default: invalid_opcode();
            }
            break;

        case 0xE:
            switch (opcode & 0x00FFu)
            {
                case 0x9E: instrEX9E(); break;
                case 0xA1: instrEXA1(); break;
                default: invalid_opcode();
            }
            break;

        case 0xF:
            switch (opcode & 0x00FFu)
            {
                case 0x07: instrFX07(); break;
                case 0x0A: instrFX0A(); break;
                case 0x15: instrFX15(); break;
                case 0x18: instrFX18(); break;
                case 0x1E: instrFX1E(); break;
                case 0x29: instrFX29(); break;
                case 0x33: instrFX33(); break;
                case 0x55: instrFX55(); break;
                case 0x65: instrFX65(); break;
                default: invalid_opcode();
            }
            break;

        default: invalid_opcode();
    }
}

void processor::invalid_opcode() const
{
    throw opcode_decoding_error(opcode);
}

void processor::instr00E0() const {}
void processor::instr00EE() noexcept {}
void processor::instr1NNN() noexcept {}
void processor::instr2NNN() noexcept {}
void processor::instr3XKK() noexcept {}
void processor::instr4XKK() noexcept {}
void processor::instr5XY0() noexcept {}
void processor::instr6XKK() noexcept {}
void processor::instr7XKK() noexcept {}
void processor::instr8XY0() noexcept {}
void processor::instr8XY1() noexcept {}
void processor::instr8XY2() noexcept {}
void processor::instr8XY3() noexcept {}
void processor::instr8XY4() noexcept {}
void processor::instr8XY5() noexcept {}
void processor::instr8XY6() noexcept {}
void processor::instr8XY7() noexcept {}
void processor::instr8XYE() noexcept {}
void processor::instr9XY0() noexcept {}
void processor::instrANNN() noexcept {}
void processor::instrBNNN() noexcept {}
void processor::instrCXKK() noexcept {}
void processor::instrDXYN() noexcept {}
void processor::instrEX9E() noexcept {}
void processor::instrEXA1() noexcept {}
void processor::instrFX0A() noexcept {}
void processor::instrFX07() noexcept {}
void processor::instrFX1E() noexcept {}
void processor::instrFX15() noexcept {}
void processor::instrFX18() noexcept {}
void processor::instrFX29() noexcept {}
void processor::instrFX33() noexcept {}
void processor::instrFX55() noexcept {}
void processor::instrFX65() noexcept {}
}