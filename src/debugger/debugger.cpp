#include <chip8emu/debugger/debugger.hpp>
#include <cassert>


void debugger::attach_process_handle(emu::chip8& chip8) noexcept
{
    m_emulator = &chip8;
    m_processor = &m_emulator->dev_bus.cpu;
    m_memory = &m_emulator->dev_bus.ram;
}

bool debugger::has_process_handle() const noexcept
{
    return m_emulator && m_processor && m_memory;
}

void debugger::check_breakpoints()
{
    if (m_breakpoints.contains(m_processor->PC))
        m_processor->set_flag(emu::proc_flags::HALT, true);
}

[[nodiscard]] u8 debugger::get_gp_reg_value(u8 reg_index) const noexcept
{
    assert(m_processor);
    return m_processor->V[reg_index];
}

[[nodiscard]] u16 debugger::get_sp_reg_value(register_type reg) const noexcept
{
    assert(m_processor);

    switch (reg)
    {
        case register_type::PC: return m_processor->PC;
        case register_type::SP: return m_processor->SP;
        case register_type::I:  return m_processor->I;
        case register_type::DT: return m_processor->DT;
        default: return 0;
    }
}

[[nodiscard]] u16 debugger::get_stack_value(u8 st_index) const noexcept
{
    assert(m_processor);
    return m_processor->stack[st_index];
}

void debugger::set_gp_reg_value(u8 reg_index, u8 value) noexcept
{
    assert(m_processor);
    m_processor->V[reg_index] = value;
}

void debugger::set_sp_reg_value(register_type reg, u16 value) noexcept
{
    assert(m_processor);

    switch (reg)
    {
        case register_type::PC: m_processor->PC = value; break;
        case register_type::SP: m_processor->SP = value; break;
        case register_type::I:  m_processor->I  = value; break;
        case register_type::DT: m_processor->DT = value; break;
    }
}

void debugger::set_stack_value(u8 st_index, u8 value) noexcept
{
    assert(m_processor);
    m_processor->stack[st_index] = value;
}

[[nodiscard]] bool debugger::add_breakpoint(address_type address)
{
    return m_breakpoints.insert(address).second;
}

[[nodiscard]] bool debugger::remove_breakpoint(address_type address)
{
    return m_breakpoints.erase(address);
}

void debugger::pause_process() const
{
    if (has_process_handle())
        m_processor->flip_flag(emu::proc_flags::HALT);
}

void debugger::step_next() const
{
    if (has_process_handle() && m_processor->is_flag_set(emu::proc_flags::HALT))
        m_emulator->execute_cpu_cycle();
}

void debugger::rerun()
{

}
