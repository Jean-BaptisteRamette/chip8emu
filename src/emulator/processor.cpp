#include <chip8emu/emulator/processor.hpp>
#include <chip8emu/emulator/memory.hpp>

namespace emulator
{

processor::processor(device_bus &bus) noexcept
    : m_bus {bus}, V {}, stack {}, PC(memory::CODE_ADDRESS)
{}

}
