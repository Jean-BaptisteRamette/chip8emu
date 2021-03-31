#ifndef CHIP8_EMU_IMGUI_WINDOW_BASE_HPP
#define CHIP8_EMU_IMGUI_WINDOW_BASE_HPP

#include <string_view>
#include <imgui/imgui.h>
#include <chip8emu/emulator/screen_display.hpp>


class imgui_window_base
{
public:
    imgui_window_base(const std::string_view& name, const ImVec2& size) : m_name(name), default_size(size) {};
    virtual ~imgui_window_base() noexcept = default;

    virtual void update() = 0;

protected:
    virtual bool begin(ImGuiWindowFlags flags = 0) const
    {
        ImGui::SetNextWindowSize(default_size, ImGuiCond_FirstUseEver);
        return ImGui::Begin(m_name.data(), nullptr, flags);
    };

    virtual void end() const
    {
        return ImGui::End();
    };

    const std::string_view m_name;
    const ImVec2 default_size;

    static constexpr ImGuiWindowFlags DEFAULT_FLAGS {
        ImGuiWindowFlags_None
    };
};

#endif //CHIP8_EMU_IMGUI_WINDOW_BASE_HPP
