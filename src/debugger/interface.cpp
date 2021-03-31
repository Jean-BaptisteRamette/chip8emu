#include <chip8emu/debugger/interface.hpp>
#include <array>


namespace dbg
{

static constexpr std::array<char[3], emu::processor::REG_COUNT> reg_names {
    "V0", "V1", "V2", "V3",
    "V4", "V5", "V6", "V7",
    "V8", "V9", "VA", "VB",
    "VC", "VD", "VE", "VF"
};


interface::interface() noexcept : imgui_window_base("Debugger", {500, 300})
{
    for (int i = 0; i < 100; ++i)
        if (!add_breakpoint(i))
        {
            // breakpoint already exists
        }
}

void interface::update()
{
    begin(DBG_INTERFACE_WINDOW_FLAGS);

    if (ImGui::BeginTabBar("DebuggerInterfaceTabBar"))
    {
        if (ImGui::BeginTabItem("Debugger"))
        {
            ImGui::Button("Prev");   ImGui::SameLine();
            ImGui::Button("Next");   ImGui::SameLine();
            ImGui::Button("Resume"); ImGui::SameLine();
            ImGui::Button("Rerun");

            if (ImGui::CollapsingHeader("Breakpoints"))
                show_breakpoints();

            if (ImGui::CollapsingHeader("Registers"))
            {
                if (ImGui::TreeNode("General Purpose"))
                {
                    /* Show modifiable general purpose registers */
                    show_general_registers();
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Special"))
                {
                    /* Show modifiable special registers */
                    show_special_registers();
                    ImGui::TreePop();
                }
            }

            if (ImGui::CollapsingHeader("Stack"))
                show_stack();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Memory Editor"))
        {
            ImGui::Text("Welcome to the memory editor !");
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    end();
}

void interface::show_breakpoints() const
{
    constexpr u8 columns_count { 2 };
    const float area_height { ImGui::GetTextLineHeight() * 20 };

    ImGui::BeginChild("BreakpointsArea", ImVec2(0, area_height));
    ImGui::Columns(columns_count);

    for (const emu::address_type breakpoint : m_breakpoints)
    {
        ImGui::Text("%hu", breakpoint);
        ImGui::NextColumn();
    }

    ImGui::EndChild();
}

void interface::show_general_registers() const
{
    /* register's name, register's value */
    constexpr u8 columns_count { 2 };
    const float area_height { ImGui::GetTextLineHeightWithSpacing() * emu::processor::REG_COUNT };

    ImGui::BeginChild("GeneralRegistersArea", ImVec2(0, area_height));
    ImGui::Columns(columns_count);

    for (u8 i {0}; i < m_processor->REG_COUNT; ++i)
    {
        ImGui::TextUnformatted(reg_names[i]);
        ImGui::NextColumn();

        ImGui::Text("%hhu", debugger::get_reg_value(i));
        ImGui::NextColumn();
    }

    ImGui::EndChild();
}

void interface::show_special_registers() const
{
    constexpr u8 columns_count { 2 };
    const float area_height { ImGui::GetTextLineHeightWithSpacing() * emu::processor::SPECIAL_REG_COUNT };

    ImGui::BeginChild("SpecialRegistersArea", ImVec2(0, area_height));
    ImGui::Columns(columns_count);

    ImGui::TextUnformatted("PC"); ImGui::NextColumn();
    ImGui::Text("%hu", get_reg_value(debugger::register_type::PC)); ImGui::NextColumn();

    ImGui::TextUnformatted("SP"); ImGui::NextColumn();
    ImGui::Text("%hu", get_reg_value(debugger::register_type::SP)); ImGui::NextColumn();

    ImGui::TextUnformatted("I"); ImGui::NextColumn();
    ImGui::Text("%hu", get_reg_value(debugger::register_type::I)); ImGui::NextColumn();

    ImGui::TextUnformatted("Timer"); ImGui::NextColumn();
    ImGui::Text("%hu", get_reg_value(debugger::register_type::Timer)); ImGui::NextColumn();

    ImGui::EndChild();
}

void interface::show_stack() const
{
    for (u8 i {0}; i < emu::processor::STACK_SIZE; ++i)
    {
        ImGui::Text("%hu", get_stack_value(i));
        ImGui::SameLine();
    }
}

}
