#ifndef CHIP8_EMU_INTERFACE_HPP
#define CHIP8_EMU_INTERFACE_HPP

#include <chip8emu/debugger/debugger.hpp>
#include <chip8emu/imgui_window_base.hpp>
#include <chip8emu/emulator/chip8.hpp>


namespace dbg
{
    class interface final : public debugger, public imgui_window_base
    {
        static constexpr ImGuiWindowFlags DBG_INTERFACE_WINDOW_FLAGS {
            DEFAULT_FLAGS
        };

    public:
        interface() noexcept;
        ~interface() noexcept = default;

        void update() override;

    private:
        void show_breakpoints() const;
        void show_general_registers() const;
        void show_special_registers() const;
        void show_stack() const;
    };
}

#endif //CHIP8_EMU_INTERFACE_HPP
