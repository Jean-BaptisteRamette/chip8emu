#ifndef CHIP8_EMU_INTERFACE_HPP
#define CHIP8_EMU_INTERFACE_HPP

#include <chip8emu/debugger/debugger.hpp>
#include <chip8emu/imgui_window_base.hpp>


namespace dbg
{
    class interface final : imgui_window_base
    {
    public:
        interface() noexcept;
        ~interface() noexcept = default;

        void update() override;

    private:
        debugger m_debugger;
    };
}

#endif //CHIP8_EMU_INTERFACE_HPP
