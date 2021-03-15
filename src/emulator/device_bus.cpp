#include <chip8emu/emulator/device_bus.hpp>

namespace emulator
{
device_bus::device_bus() noexcept : cpu(*this) {}
}
