#include <chip8emu/debugger/memory_editor.hpp>
#include <array>
#include <imgui/imgui.h>


namespace dbg
{
void memory_editor::show(emu::memory& memory) noexcept
{
    ImGui::BeginChild("Memory Editor");

    show_options(memory);
    show_memory_view(memory);

    ImGui::EndChild();
}

void memory_editor::show_memory_view(emu::memory& memory) noexcept
{
    ImGui::BeginChild("View");

    update_addr_cursor_pos();

    /* display a whole range of memory */
    for (u8 row { 0 }; row < DISPLAY_ROWS; ++row)
        show_memory_row(memory, row);

    ImGui::EndChild();
}

void memory_editor::show_memory_row(emu::memory& memory, int row) noexcept
{
    u16 row_start_addr = m_curr_range_start + row * BYTES_PER_ROW;
    ImGui::Text("%04X: ", row_start_addr);

    std::array<char, BYTES_PER_ROW + 1> ascii_buffer;
    ascii_buffer[16] = '\0';

    address_type ptr { row_start_addr };
    for (u8 byte_index = ptr - row_start_addr; byte_index < BYTES_PER_ROW && ptr < emu::memory::SIZE; ++byte_index, ++ptr)
    {
        ImGui::SameLine();

        if (ptr == m_cursor_addr)
            ImGui::TextColored({.0f, 1.0f, .0f, 1.0f}, "%02X", memory[ptr]);
        else
            ImGui::Text("%02X", memory[ptr]);

        if (memory[ptr] > 32 && memory[ptr] < 127)
            ascii_buffer[byte_index] = memory[ptr];
        else
            ascii_buffer[byte_index] = '.';

        if (ImGui::IsItemClicked())
            m_cursor_addr = ptr;
    }

    if (!m_show_ascii)
        return;

    for (u8 i {}; i < ascii_buffer.size() - 1; ++i)
    {
        ImGui::SameLine();

        if (row_start_addr + i == m_cursor_addr)
            ImGui::TextColored({.0f, 1.0f, .0f, 1.0f}, "%c", ascii_buffer[i]);
        else
            ImGui::Text("%c", ascii_buffer[i]);
    }
}

void memory_editor::update_addr_cursor_pos() noexcept
{
    /* show cursor position with arrow keys */
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftArrow)) && m_cursor_addr)
        m_cursor_addr -= 1;
    else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightArrow)) && m_cursor_addr < emu::memory::SIZE - 1)
        m_cursor_addr += 1;
    else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow)) && m_cursor_addr >= BYTES_PER_ROW)
        m_cursor_addr -= BYTES_PER_ROW;
    else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow)) && m_cursor_addr < emu::memory::SIZE - BYTES_PER_ROW)
        m_cursor_addr += BYTES_PER_ROW;

    if (m_cursor_addr < m_curr_range_start)
        mem_range_goto(-1);

    else if (m_cursor_addr >= m_curr_range_end)
        mem_range_goto(1);

    if (m_scroll_offset)
    {
        ImGui::SetScrollY(ImGui::GetScrollY() + m_scroll_offset);
        m_scroll_offset = 0;
    }
}

void memory_editor::show_options(emu::memory& memory) noexcept
{
    ImGui::Checkbox("Read Only", &m_readonly);
    ImGui::SameLine(0, 20);

    ImGui::Checkbox("Ascii", &m_show_ascii);
    ImGui::SameLine(0, 20);

    if (ImGui::ArrowButton("prev", ImGuiDir_Left) && m_curr_range_start >= RANGE_SIZE)
    {
        m_cursor_addr -= RANGE_SIZE;
        mem_range_goto(-1);
    }

    ImGui::SameLine();
    ImGui::Text("%04X-%04X", m_curr_range_start, m_curr_range_end);
    ImGui::SameLine();

    if (ImGui::ArrowButton("next", ImGuiDir_Right) && m_curr_range_end <= emu::memory::SIZE - RANGE_SIZE)
    {
        m_cursor_addr += RANGE_SIZE;
        mem_range_goto(1);
    }

    ImGui::SameLine(0, 20);

    const address_type previous_addr { m_cursor_addr };
    ImGui::SetNextItemWidth(40);
    if (ImGui::InputScalar("Address", ImGuiDataType_U16, &m_cursor_addr, nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal))
    {
        if (m_cursor_addr >= emu::memory::SIZE)
            m_cursor_addr = emu::memory::SIZE - 1;

        // Is the new address out of the current range ?
        if (m_cursor_addr < m_curr_range_start || m_cursor_addr >= m_curr_range_end)
        {
            const int lines_difference { (m_cursor_addr / DISPLAY_ROWS) - (previous_addr / DISPLAY_ROWS) };
            const int range_difference { lines_difference / DISPLAY_ROWS };

            mem_range_goto(range_difference);
        }
    }

    if (!m_readonly)
    {
        ImGui::SameLine(0, 20);
        ImGui::SetNextItemWidth(50);
        ImGui::InputScalar("Value", ImGuiDataType_U8, &memory[m_cursor_addr]);
    }
}

void memory_editor::mem_range_goto(int relative_count) noexcept
{
    const auto addr_difference { relative_count * RANGE_SIZE };

    m_curr_range_start += addr_difference;
    m_curr_range_end += addr_difference;

    if (relative_count > 0)
        m_scroll_offset = static_cast<int>(DISPLAY_ROWS * ImGui::GetTextLineHeight());
    else
        m_scroll_offset = static_cast<int>(-DISPLAY_ROWS * ImGui::GetTextLineHeight());
}
}
