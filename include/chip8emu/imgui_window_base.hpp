#ifndef CHIP8_EMU_IMGUI_WINDOW_BASE_HPP
#define CHIP8_EMU_IMGUI_WINDOW_BASE_HPP

#include <string_view>
#include <imgui/imgui.h>

class imgui_window_base
{
public:
    imgui_window_base(const std::string_view& name) : m_name(name) {};
    virtual ~imgui_window_base() noexcept = default;
    virtual void update() = 0;

protected:
    virtual bool begin() const
    {
        return ImGui::Begin(m_name.data());
    };

    virtual void end() const
    {
        return ImGui::End();
    };

    const std::string_view m_name;
};

#endif //CHIP8_EMU_IMGUI_WINDOW_BASE_HPP
