#include <chip8emu/console/console.hpp>

namespace csl
{

console::console() noexcept : imgui_window_base("Console", {POS_X, POS_Y}, {WIDTH, HEIGHT})
{}

void console::update()
{
    begin(CONSOLE_WINDOW_FLAGS);
    ImGui::Button("Test Button");
    end();
}

}
