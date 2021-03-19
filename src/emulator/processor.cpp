#include <chip8emu/emulator/screen_display.hpp>
#include <chip8emu/emulator/device_bus.hpp>
#include <chip8emu/emulator/processor.hpp>
#include <chip8emu/emulator/memory.hpp>

namespace emulator
{
#define OPCODE_GETX(opcode) ((opcode) & 0x0F00u) >> 8u  // OPCODE_GETX(0x8F13) -> 0xF
#define OPCODE_GETY(opcode) ((opcode) & 0x00F0u) >> 4u  // OPCODE_GETY(0x8F13) -> 0x1
#define JMP_NEXT(pc) pc += 2  // jumps to the next instruction

processor::processor(device_bus &bus) noexcept
        : m_bus{bus},
        mt(std::random_device()()), distribution(0, 255), // PRNG
        V{}, stack{}, PC(memory::CODE_ADDRESS)
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

void processor::instr00E0() const
{
    /* clear screen memory */
    m_bus.screen.clear();
}

void processor::instr00EE() noexcept
{
    /* return from a subroutine */
    --SP;
    PC = stack[SP];
}

void processor::instr1NNN() noexcept
{
    /* jumps to instruction at address NNN */
    PC = opcode & 0x0FFFu;
}
void processor::instr2NNN() noexcept
{
    /* return from a subroutine */
    stack[SP] = PC;
    ++SP;
    PC = opcode & 0x0FFFu;
}

void processor::instr3XKK() noexcept
{
    /* if the value of register VX isn't equal to the value KK, skip instruction */
    const auto VX { V[OPCODE_GETX(opcode)] };
    const auto KK { opcode & 0xFFu };

    if (VX == KK)
        JMP_NEXT(PC);
}
void processor::instr4XKK() noexcept
{
    /* if the value of register X isn't equal to the value KK, skip instruction */
    const auto VX { V[OPCODE_GETX(opcode)] };
    const auto KK { opcode & 0x00FFu };

    if (VX != KK)
        JMP_NEXT(PC);
}
void processor::instr5XY0() noexcept
{
    /* if register X and register Y are equal, skip instruction */
    const auto VX { V[OPCODE_GETX(opcode)] };
    const auto VY { V[OPCODE_GETY(opcode)] };

    if (VX == VY)
        JMP_NEXT(PC);
}

void processor::instr6XKK() noexcept
{
    /* Sets register VX to the value KK */
    V[OPCODE_GETX(opcode)] = opcode & 0x00FFu;
}

void processor::instr7XKK() noexcept
{
    /* Adds the value KK to register X */
    V[OPCODE_GETX(opcode)] += opcode & 0x00FFu;
}

void processor::instr8XY0() noexcept
{
    /* Sets VX to VY */
    V[OPCODE_GETX(opcode)] = V[OPCODE_GETY(opcode)];
}

void processor::instr8XY1() noexcept
{
    /* Sets VX to VX or VY */
    V[OPCODE_GETX(opcode)] |= V[OPCODE_GETY(opcode)];
}

void processor::instr8XY2() noexcept
{
    /* Sets VX to VX and VY */
    V[OPCODE_GETX(opcode)] &= V[OPCODE_GETY(opcode)];
}

void processor::instr8XY3() noexcept
{
    /* Sets VX to VX xor VY */
    V[OPCODE_GETX(opcode)] ^= V[OPCODE_GETY(opcode)];
}

void processor::instr8XY4() noexcept
{
    /* Adds values of register X and register Y then set carry bit if needed */
    u8 &VX { V[OPCODE_GETX(opcode)] };
    const u8 &VY { V[OPCODE_GETY(opcode)] };

    const auto result { static_cast<u16>(VX + VY) };

    // Set the carry bit
    V[0xF] = result > 255;

    // Keep lowest byte
    VX = result & 0xFFu;
}

void processor::instr8XY5() noexcept
{
    /* Subtracts register Y from register X and sets carry flag */
    const auto X { OPCODE_GETX(opcode) };
    const auto Y { OPCODE_GETY(opcode) };

    V[0xF] = V[X] > V[Y];
    V[X] -= V[Y];
}

void processor::instr8XY6() noexcept
{
    // Sets VX to VX divided by two, and set carry bit if needed:
    // as we shift by one to the right, carry flag is set if the LSB is 1
    const auto X { OPCODE_GETX(opcode) };

    V[0xF] = V[X] & 1u;
    V[X] >>= 1u;
}

void processor::instr8XY7() noexcept
{
    /* Sets register X to VY - VX and sets carry flag if needed */
    const auto X { OPCODE_GETX(opcode) };
    const auto Y { OPCODE_GETY(opcode) };

    V[0xF] = V[Y] > V[X];
    V[X] = V[Y] - V[X];
}

void processor::instr8XYE() noexcept
{
    // Sets VX to VX multiplied by two, and set carry bit if needed:
    // as we shift by one to the left, carry flag is set if the MSB is 1
    const auto X { OPCODE_GETX(opcode) };

    V[0xF] = V[X] & 0x80u;
    V[X] <<= 1u;
}

void processor::instr9XY0() noexcept
{
    /* Skips the next instruction if register X isn't equal to register Y */
    const u16 VX { V[OPCODE_GETX(opcode)] };
    const u16 VY { V[OPCODE_GETY(opcode)] };

    if (VX != VY)
        JMP_NEXT(PC);
}

void processor::instrANNN() noexcept
{
    /* Sets I to the address NNN */
    I = opcode & 0x0FFFu;
}

void processor::instrBNNN() noexcept
{
    /* Jumps to the address V0 + NNN */
    PC = V[0] + (opcode & 0x0FFFu);
}

void processor::instrCXKK() noexcept
{
    // Sets register X to a random number a
    // 0 <= a <= 255 then AND a with the value KK
    const auto X  { OPCODE_GETX(opcode) };
    const auto KK { static_cast<u8>(opcode & 0xFFu) };
    V[X] = distribution(mt) & KK;
}

void processor::instrDXYN() noexcept
{
    /* Draw sprite at coordinates VX, VY */
    const u8 x = V[OPCODE_GETX(opcode)] % screen_display::WIDTH;
    const u8 y = V[OPCODE_GETY(opcode)] % screen_display::HEIGHT;
    const u8 height = opcode & 0xFu;

    /* reset carry flag register for collision detection */
    V[0xF] = 0;

    for (u8 row {}; row < height; ++row)
    {
        /* get sprite row to draw */
        const u8 sprite_row { m_bus.ram[I + row] };

        /* loop through bits from MSB to LSB */
        for (u8 col {}; col < 8; ++col)
        {
            /* x + y * w formula */
            const u64 pos = (x + col) + screen_display::WIDTH * (y + row);

            /* get video pixel to copy_video_buffer */
            u32& pixel {m_bus.screen.pixel(pos) };

            if (sprite_row & (0x80u >> col))
            {
                /* set carry flag to check for collision if the previous pixel was already on */
                V[0xF] = (pixel == 0xFFFFFFFF);

                /* xor draw */
                pixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void processor::instrEX9E() noexcept
{
    /* Skips instruction if key VX is pressed */
    const auto key_index { V[OPCODE_GETX(opcode)] };

    if (m_bus.keypad.key_pressed(key_index))
        JMP_NEXT(PC);
}

void processor::instrEXA1() noexcept
{
    /* Skips instruction if key is not pressed */
    const auto key_index { V[OPCODE_GETX(opcode)] };

    if (!m_bus.keypad.key_pressed(key_index))
        JMP_NEXT(PC);
}

    void processor::instrFX07() noexcept
{
    /* Sets VX to the value of the delay timer */
    V[OPCODE_GETX(opcode)] = delay_timer;
}

void processor::instrFX0A() noexcept
{
    /* Waits for a key to be pressed and stores it in VX */
    auto& VX { V[OPCODE_GETX(opcode)] };

    for (u8 key_index{0}; key_index < keyboard::KEY_COUNT; ++key_index)
    {
        if (m_bus.keypad.key_pressed(key_index))
        {
            VX = key_index;
            return;
        }
    }

    // Repeat instruction
    PC -= 2;
}

void processor::instrFX15() noexcept
{
    /* Set delay timer to VX */
    delay_timer = V[OPCODE_GETX(opcode)];
}

void processor::instrFX18() noexcept
{
    /* Set sound timer to VX */
}

void processor::instrFX1E() noexcept
{
    /* Adds VX to I */
    I += V[OPCODE_GETX(opcode)];
}

void processor::instrFX29() noexcept
{
    /* Sets I to the location of the glyph corresponding to the digit of value VX */
    const auto digit { V[OPCODE_GETX(opcode)] };
    const auto digit_glyph_offset { memory::FONT_ADDRESS + digit * memory::FONT_CHAR_SIZE };

    I = static_cast<u16>(digit_glyph_offset);
}

void processor::instrFX33() noexcept
{
    /* Stores BCD representation */
    auto VX { V[OPCODE_GETX(opcode)] };

    m_bus.ram[I + 2] = VX % 10;
    VX /= 10;                    // Ones place, (1st digit from the right)
    m_bus.ram[I + 1] = VX % 10;
    VX /= 10;                    // Tens place, (2nd digit from the right)
    m_bus.ram[I] = VX % 10;      // Hundreds place, (3rd digit from the right)
}

void processor::instrFX55() noexcept
{
    /* Dump registers values in memory from address I */
    const auto X { OPCODE_GETX(opcode) };

    for (u8 reg{0}; reg <= X; ++reg)
        m_bus.ram[I + reg] = V[reg];
}

void processor::instrFX65() noexcept
{
    /* Fills register from V0 to VX with values from memory starting at address I */
    const auto X { OPCODE_GETX(opcode) };

    for (u8 reg {}; reg <= X; ++reg)
        V[reg] = m_bus.ram[I + reg];
}
}