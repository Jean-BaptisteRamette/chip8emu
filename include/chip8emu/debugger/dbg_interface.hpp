#ifndef CHIP8_EMU_DBG_INTERFACE_HPP
#define CHIP8_EMU_DBG_INTERFACE_HPP

#include <chip8emu/emulator/screen_display.hpp>
#include <chip8emu/debugger/memory_editor.hpp>
#include <chip8emu/debugger/debugger.hpp>
#include <chip8emu/imgui_window_base.hpp>
#include <chip8emu/emulator/chip8.hpp>


namespace dbg
{

    class dbg_interface final : public debugger, public imgui_window_base
    {
        static constexpr ImGuiWindowFlags DBG_INTERFACE_WINDOW_FLAGS {
            DEFAULT_FLAGS
        };

        static constexpr float POS_X { 0 };
        static constexpr float POS_Y { emu::screen_display::SCALED_HEIGHT };

        memory_editor mem_edit;

    public:
        static constexpr float WIDTH  { emu::screen_display::SCALED_WIDTH };
        static constexpr float HEIGHT { 400 };

    public:
        dbg_interface() noexcept;
        ~dbg_interface() noexcept = default;
        dbg_interface(const dbg_interface&) = delete;
        dbg_interface(dbg_interface&&) = delete;
        dbg_interface& operator=(const dbg_interface&) = delete;
        dbg_interface& operator=(dbg_interface&&) = delete;

        void show() override;

    private:
        void show_breakpoints() const;
        void show_registers();
        void show_general_registers();
        void show_special_registers();
        void show_stack();
    };
}

#endif //CHIP8_EMU_DBG_INTERFACE_HPP
