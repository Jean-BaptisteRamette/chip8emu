#include <chip8emu/debugger/dbg_interface.hpp>
#include <string_view>
#include <array>


namespace dbg
{

static constexpr std::array<std::string_view, emu::processor::REG_COUNT> reg_names {
    "R0", "R1", "R2", "R3",
    "R4", "R5", "R6", "R7",
    "R8", "R9", "RA", "RB",
    "RC", "RD", "RE", "RF"
};


dbg_interface::dbg_interface() noexcept
    : imgui_window_base("Debugger", {POS_X, POS_Y}, {WIDTH, HEIGHT})
{}

void dbg_interface::update()
{
    check_breakpoints();

    begin(DBG_INTERFACE_WINDOW_FLAGS);

    if (ImGui::BeginTabBar("DebuggerInterfaceTabBar"))
    {
        if (ImGui::BeginTabItem("Debugger"))
        {
            if (ImGui::Button("Pause")) pause_process(); ImGui::SameLine();
            if (ImGui::Button("Next")) step_next(); ImGui::SameLine();
            if (ImGui::Button("Rerun")) {}

            if (ImGui::CollapsingHeader("Breakpoints")) show_breakpoints();
            if (ImGui::CollapsingHeader("Registers")) show_registers();
            if (ImGui::CollapsingHeader("Stack")) show_stack();

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

void dbg_interface::show_breakpoints() const
{
    const float area_height { ImGui::GetTextLineHeight() * (m_breakpoints.size() + 1) / 2 };

    ImGui::BeginChild("BreakpointsArea", ImVec2(0, area_height));
    ImGui::Columns(2);

    for (const emu::address_type breakpoint : m_breakpoints)
    {
        ImGui::Text("%hu", breakpoint);
        ImGui::NextColumn();
    }

    ImGui::EndChild();
}

void dbg_interface::show_registers()
{
    if (ImGui::TreeNode("General Purpose"))
    {
        show_general_registers();
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Special"))
    {
        show_special_registers();
        ImGui::TreePop();
    }
}

void dbg_interface::show_general_registers()
{
    const float area_height { ImGui::GetTextLineHeightWithSpacing() * emu::processor::REG_COUNT };

    ImGui::BeginChild("GeneralRegistersArea", ImVec2(0, area_height));

    /* register's name, register's value */
    ImGui::Columns(2);

    if (!has_process_handle())
        return;

    if (m_processor->is_flag_set(emu::proc_flags::HALT))
    {
        for (u8 i {}; i < m_processor->REG_COUNT; ++i)
        {
            ImGui::TextUnformatted(reg_names[i].data());
            ImGui::NextColumn();

            auto regv { get_gp_reg_value(i) };

            ImGui::PushID(i);
            if (ImGui::InputScalar("", ImGuiDataType_U8, &regv))
                set_gp_reg_value(i, regv);
            ImGui::PopID();

            ImGui::NextColumn();
        }
    } else
    {
        for (u8 i {}; i < m_processor->REG_COUNT; ++i)
        {
            ImGui::TextUnformatted(reg_names[i].data());
            ImGui::NextColumn();

            ImGui::Text("%hhu", debugger::get_gp_reg_value(i));
            ImGui::NextColumn();
        }
    }

    ImGui::EndChild();
}

void dbg_interface::show_special_registers()
{
    constexpr u8 columns_count { 2 };
    const float area_height { ImGui::GetTextLineHeightWithSpacing() * emu::processor::SPECIAL_REG_COUNT };

    ImGui::BeginChild("SpecialRegistersArea", ImVec2(0, area_height));
    ImGui::Columns(columns_count);

    if (!has_process_handle())
        return;

    u16 reg_pc { get_sp_reg_value(register_type::PC) };
    u16 reg_sp { get_sp_reg_value(register_type::SP) };
    u16 reg_i  { get_sp_reg_value(register_type::I)  };
    u8  reg_dt = get_sp_reg_value(register_type::DT);

    if (m_processor->is_flag_set(emu::proc_flags::HALT))
    {
        ImGui::TextUnformatted("PC"); ImGui::NextColumn();
        if (ImGui::InputScalar("##1", ImGuiDataType_U16, &reg_pc))
            set_sp_reg_value(register_type::PC, reg_pc);
        ImGui::NextColumn();

        ImGui::TextUnformatted("SP"); ImGui::NextColumn();
        if (ImGui::InputScalar("##2", ImGuiDataType_U16, &reg_sp))
            set_sp_reg_value(register_type::SP, reg_sp);
        ImGui::NextColumn();

        ImGui::TextUnformatted("I"); ImGui::NextColumn();
        if (ImGui::InputScalar("##3", ImGuiDataType_U16, &reg_i))
            set_sp_reg_value(register_type::I, reg_i);
        ImGui::NextColumn();

        ImGui::TextUnformatted("Timer"); ImGui::NextColumn();
        if (ImGui::InputScalar("##4", ImGuiDataType_U8, &reg_dt))
            set_sp_reg_value(register_type::DT, reg_dt);
        ImGui::NextColumn();

    } else
    {
        ImGui::TextUnformatted("PC"); ImGui::NextColumn();
        ImGui::Text("%hu", reg_pc); ImGui::NextColumn();

        ImGui::TextUnformatted("SP"); ImGui::NextColumn();
        ImGui::Text("%hu", reg_sp); ImGui::NextColumn();

        ImGui::TextUnformatted("I"); ImGui::NextColumn();
        ImGui::Text("%hu", reg_i); ImGui::NextColumn();

        ImGui::TextUnformatted("Timer"); ImGui::NextColumn();
        ImGui::Text("%hhu", reg_dt); ImGui::NextColumn();
    }

    ImGui::EndChild();
}

void dbg_interface::show_stack()
{
    if (!has_process_handle())
        return;

    ImGui::Columns(2);

    if (m_processor->is_flag_set(emu::proc_flags::HALT))
    {
        for (u8 i {}; i < emu::processor::STACK_SIZE; ++i)
        {
            ImGui::PushID(i);

            auto stack_v { get_stack_value(i) };
            ImGui::Text("ST%hhu", i); ImGui::NextColumn();
            if (ImGui::InputScalar("", ImGuiDataType_U16, &stack_v))
                set_stack_value(i, stack_v);

            ImGui::NextColumn();

            ImGui::PopID();
        }
    } else
    {
        for (u8 i {}; i < emu::processor::STACK_SIZE; ++i)
        {
            ImGui::Text("ST%hhu", i); ImGui::NextColumn();
            ImGui::Text("%hu", get_stack_value(i)); ImGui::NextColumn();
        }
    }
}
}
