#include <chip8emu/emulator/screen_display.hpp>


namespace emulator
{
    screen_display::screen_display()
    {
        m_video_buffer.resize(WIDTH * HEIGHT);
    }

    void screen_display::clear()
    {
        std::fill(std::begin(m_video_buffer), std::end(m_video_buffer), 0);
    }

    void screen_display::copy_video_buffer(SDL_Renderer* renderer, SDL_Texture* texture) const noexcept
    {
        SDL_UpdateTexture(texture, nullptr, std::data(m_video_buffer), PITCH);
        SDL_RenderCopy(renderer, texture, nullptr, &m_render_target);
    }

    u32 &screen_display::pixel(u64 pos) noexcept
    {
        return m_video_buffer[pos];
    }
}
