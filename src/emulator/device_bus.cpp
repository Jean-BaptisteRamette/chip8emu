#include <chip8emu/emulator/device_bus.hpp>

namespace emu
{
device_bus::device_bus(const std::shared_ptr<SDL_Renderer>& renderer_ptr) noexcept :
    cpu(*this), screen(renderer_ptr) {}
}
