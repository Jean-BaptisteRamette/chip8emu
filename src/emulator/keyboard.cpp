#include <chip8emu/emulator/keyboard.hpp>


namespace emulator
{
    void keyboard::update_state(SDL_Keycode keycode, bool pressed) noexcept
    {
        const auto key_it { keymap.find(keycode) };
        if (key_it != keymap.end())
        {
            const u8 key_index { key_it->second };
            m_keys[key_index] = pressed;
        }
    }

    [[nodiscard]] bool keyboard::key_pressed(u8 key_index) const noexcept
    {
        return m_keys[key_index];
    }
}
