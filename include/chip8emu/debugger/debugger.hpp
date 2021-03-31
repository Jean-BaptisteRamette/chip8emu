#ifndef CHIP8_EMU_DEBUGGER_HPP
#define CHIP8_EMU_DEBUGGER_HPP

#include <chip8emu/emulator/chip8.hpp>
#include <chip8emu/emulator/processor.hpp>
#include <chip8emu/emulator/memory.hpp>
#include <set>


/*
 * Can't put this in the dbg namespace because it needs to be friend with the processor class
 * located in another namespace
 */

class debugger
{

public:
    enum class register_type { PC, SP, I, Timer };

    debugger() noexcept = default;
    virtual ~debugger() noexcept = default;
    debugger(const debugger &) = delete;
    debugger(debugger &&) = delete;
    debugger &operator=(const debugger &) = delete;
    debugger &operator=(debugger &&) = delete;

    void attach_emulator_process(emu::chip8& chip8) noexcept;

    [[nodiscard]] u8  get_reg_value(u8 reg) const noexcept;
    [[nodiscard]] u16 get_reg_value(register_type reg) const noexcept;
    [[nodiscard]] u16 get_stack_value(u8 index) const noexcept;

    /* try to insert a breakpoint, if it already exists, return false */
    [[nodiscard]] bool add_breakpoint(emu::address_type address);

    /* remove a breakpoint, if failed, return false */
    [[nodiscard]] bool remove_breakpoint(emu::address_type address);

    /* Go to next instruction */
    void step_next();

    /* Continue execution until breakpoint */
    void resume();

    /* Rerun the program */
    void rerun();

protected:
    emu::processor *m_processor;
    emu::memory *m_memory;

    /* breakpoints are sorted from lowest address to highest address */
    std::set<emu::address_type> m_breakpoints;
};

#endif //CHIP8_EMU_DEBUGGER_HPP
