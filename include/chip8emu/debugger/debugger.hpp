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
    enum class register_type
    {
        PC, SP, I, DT
    };

    debugger() noexcept = default;
    virtual ~debugger() noexcept = default;
    debugger(const debugger &) = delete;
    debugger(debugger &&) = delete;
    debugger &operator=(const debugger &) = delete;
    debugger &operator=(debugger &&) = delete;

    void attach_process_handle(emu::chip8& chip8) noexcept;
    [[nodiscard]] bool has_process_handle() const noexcept;

    void check_breakpoints();

    [[nodiscard]] u8  get_gp_reg_value(u8 reg_index) const noexcept;
    [[nodiscard]] u16 get_sp_reg_value(register_type reg) const noexcept;
    [[nodiscard]] u16 get_stack_value(u8 st_index) const noexcept;

    void set_gp_reg_value(u8 reg_index, u8 value) noexcept;
    void set_sp_reg_value(register_type reg, u16 value) noexcept;
    void set_stack_value(u8 st_index, u8 value) noexcept;

    /* try to insert a breakpoint, if it already exists, return false */
    [[nodiscard]] bool add_breakpoint(address_type address);

    /* remove a breakpoint, if failed, return false */
    [[nodiscard]] bool remove_breakpoint(address_type address);

    /* pause or unpause the current process */
    void pause_process() const;

    /* Go to next instruction */
    void step_next() const;

    /* Rerun the program */
    void rerun();

protected:
    emu::chip8* m_emulator { nullptr };
    emu::processor *m_processor { nullptr };
    emu::memory *m_memory { nullptr };

    /* breakpoints are sorted from lowest address to highest address */
    std::set<address_type> m_breakpoints;
};

#endif //CHIP8_EMU_DEBUGGER_HPP
