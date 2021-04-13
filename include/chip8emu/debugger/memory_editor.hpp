#ifndef CHIP8_EMU_MEMORY_EDITOR_HPP
#define CHIP8_EMU_MEMORY_EDITOR_HPP

#include <chip8emu/emulator/memory.hpp>
#include <chip8emu/inttypes.hpp>
#include <imgui/imgui.h>


namespace dbg
{
    class memory_editor final
    {
    private:
        /* displays up to 16 rows of 16 byte (256 bytes per range) */
        static constexpr int DISPLAY_ROWS  { 16 };
        static constexpr int BYTES_PER_ROW { 16 };
        static constexpr address_type RANGE_SIZE { DISPLAY_ROWS * BYTES_PER_ROW };

        address_type m_curr_range_start { 0x0000 };
        address_type m_curr_range_end   { RANGE_SIZE };

        /* address of the selected memory data */
        address_type m_cursor_addr { m_curr_range_start };

        int m_scroll_offset {};

        /* is the memory blocked from writing */
        bool m_readonly { true };
        bool m_show_ascii { true };

    public:
        memory_editor() noexcept = default;
        ~memory_editor() noexcept = default;
        memory_editor(const memory_editor&) = delete;
        memory_editor(memory_editor&&) = delete;
        memory_editor& operator=(const memory_editor&) = delete;
        memory_editor& operator=(memory_editor&&) = delete;

        /* we can't directly pass the memory object because it comes from another namespace */
        void show(emu::memory& memory) noexcept;

    private:
        void show_memory_view(emu::memory& memory) noexcept;
        void show_memory_row(emu::memory& memory, int row) noexcept;
        void show_options(emu::memory& memory) noexcept;

        /* checks keyboard input and offsets m_cursor_addr */
        void update_addr_cursor_pos() noexcept;

        /* jumps to a range relative to the current range */
        void mem_range_goto(int relative_count) noexcept;
    };
}

#endif //CHIP8_EMU_MEMORY_EDITOR_HPP
