#include <chip8emu/console/console.hpp>

namespace csl
{
console::console() noexcept : imgui_window_base("Console", {200, 600})
{}

void console::update()
{
    begin(CONSOLE_WINDOW_FLAGS);
    ImGui::Button("Test Button");
    end();
}
}
