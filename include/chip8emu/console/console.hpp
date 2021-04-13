#ifndef CHIP8_EMU_CONSOLE_HPP
#define CHIP8_EMU_CONSOLE_HPP

#include <chip8emu/imgui_window_base.hpp>
#include <chip8emu/console/command.hpp>
#include <queue>


/* Interactive console.
 * Commands:
 *      load <path_to_game>                  (insert game cartridge inside chip8)
 *      start                                (start the loaded game)
 *      pause                                (pause the game process)
 *      unload                               (remove the game for the chip8 cartridge)
 *      width <w>                            (sets the window's width)
 *      height <h>                           (sets the window's height)
 *      bcolor <r, g, b | 0x[0-F]{6}>        (set the background color of the chip8 display)
 *      fcolor <r, g, b | 0x[0-F]{6}>        (set the foreground color of the chip8 display)
 *      mute                                 (mute the emulated sound)
 *      unmute                               (unmute the emulated sound)
 *      quit                                 (quit the m_emulator)
 *      list                                 (list files and directories)
 *      disassemble <chip8 executable file>  (disassemble a file)
 *      assemble    <chip8 source file>      (assemble a file)
 *      clear                                (clear output)
 *      history                              (show last commands)
 */

namespace csl
{
    class console final : public imgui_window_base
    {
        static constexpr ImGuiWindowFlags CONSOLE_WINDOW_FLAGS {
            DEFAULT_FLAGS
        };

        static constexpr float POS_X { emu::screen_display::SCALED_WIDTH };
        static constexpr float POS_Y { 0 };

    public:
        console() noexcept;

        void show() override;

        static constexpr float WIDTH  { 400 };
        static constexpr float HEIGHT { emu::screen_display::SCALED_HEIGHT };

    private:
        std::queue<command> commands;
    };
}

#endif //CHIP8_EMU_CONSOLE_HPP
