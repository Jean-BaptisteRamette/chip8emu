#ifndef CHIP8_EMU_KEYBOARD_HPP
#define CHIP8_EMU_KEYBOARD_HPP

#include <SDL2/SDL_keycode.h>
#include <chip8emu/inttypes.hpp>
#include <unordered_map>
#include <array>


namespace emulator
{
    class keyboard final
    {
    public:
        static constexpr u8 KEY_COUNT { 16 };

        keyboard() noexcept = default;
        ~keyboard() noexcept = default;
        keyboard(const keyboard &) = delete;
        keyboard(keyboard &&) = delete;
        keyboard &operator=(const keyboard &) = delete;
        keyboard &operator=(keyboard &&) = delete;

        /* void set_keymap(); */
        void update_state(SDL_Keycode keycode, bool pressed) noexcept;
        [[nodiscard]] bool key_pressed(u8 key_index) const noexcept;

    private:
        /* holds the state of the keyboard */
        std::array<bool, KEY_COUNT> m_keys;

        /* physical keyboard to emulated keyboard mapping */
        std::unordered_map<SDL_Keycode, u8> keymap {
                { SDLK_x, 0x0 }, { SDLK_1, 0x1 },
                { SDLK_2, 0x2 }, { SDLK_3, 0x3 },
                { SDLK_q, 0x4 }, { SDLK_w, 0x5 },
                { SDLK_e, 0x6 }, { SDLK_a, 0x7 },
                { SDLK_s, 0x8 }, { SDLK_d, 0x9 },
                { SDLK_z, 0xA }, { SDLK_c, 0xB },
                { SDLK_4, 0xC }, { SDLK_r, 0xD },
                { SDLK_f, 0xE }, { SDLK_v, 0xF },
        };
    };
}

#endif //CHIP8_EMU_KEYBOARD_HPP
