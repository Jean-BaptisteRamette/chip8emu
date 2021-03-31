#include <chip8emu/debugger/debugger.hpp>

void debugger::attach_emulator_process(emu::chip8& chip8) noexcept
{
    m_processor = &chip8.dev_bus.cpu;
    m_memory = &chip8.dev_bus.ram;
}

[[nodiscard]] u8 debugger::get_reg_value(u8 reg) const noexcept
{
    return m_processor->V[reg];
}

[[nodiscard]] u16 debugger::get_reg_value(register_type reg) const noexcept
{
    switch (reg)
    {
        case register_type::PC:    return m_processor->PC;
        case register_type::SP:    return m_processor->SP;
        case register_type::I:     return m_processor->I;
        case register_type::Timer: return m_processor->delay_timer;
        default: return 0;
    }
}

[[nodiscard]] u16 debugger::get_stack_value(u8 index) const noexcept
{
    return m_processor->stack[index];
}

[[nodiscard]] bool debugger::add_breakpoint(emu::address_type address)
{
    return m_breakpoints.insert(address).second;
}

[[nodiscard]] bool debugger::remove_breakpoint(emu::address_type address)
{
    return m_breakpoints.erase(address);
}

void debugger::step_next()
{

}

void debugger::resume()
{

}

void debugger::rerun()
{

}
