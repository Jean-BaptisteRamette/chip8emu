#include <chip8emu/debugger/interface.hpp>


namespace dbg
{
interface::interface() noexcept : imgui_window_base("Debugger")
{}

void interface::update()
{
    begin();
    ImGui::Button("Click Me");
    end();
}
}
