#ifndef CHIP8_EMU_IMGUI_WINDOW_BASE_HPP
#define CHIP8_EMU_IMGUI_WINDOW_BASE_HPP

#include <string_view>
#include <imgui/imgui.h>
#include <chip8emu/inttypes.hpp>
#include <chip8emu/emulator/screen_display.hpp>


class imgui_window_base
{
public:
    imgui_window_base(const std::string_view& name, const ImVec2& pos, const ImVec2& size)
    : m_name(name), m_pos(pos), m_size(size) {};

    virtual ~imgui_window_base() noexcept = default;

    virtual void update() = 0;

protected:
    virtual bool begin(ImGuiWindowFlags flags = 0) const
    {
        ImGui::SetNextWindowPos(m_pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(m_size, ImGuiCond_FirstUseEver);

        return ImGui::Begin(m_name.data(), nullptr, flags);
    };

    virtual void end() const
    {
        return ImGui::End();
    };

    const std::string_view m_name;
    const ImVec2 m_pos;
    const ImVec2 m_size;

    static constexpr ImGuiWindowFlags DEFAULT_FLAGS {
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
    };
};

#endif //CHIP8_EMU_IMGUI_WINDOW_BASE_HPP
