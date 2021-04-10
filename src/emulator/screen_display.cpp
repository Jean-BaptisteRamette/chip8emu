#include <chip8emu/emulator/screen_display.hpp>


namespace emu
{
    screen_display::screen_display(const std::shared_ptr<SDL_Renderer>& renderer) :
        m_renderer(renderer),
        m_texture {
                    SDL_CreateTexture(m_renderer.get(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT),
                    SDLTextureDestroyer() }
    {
        m_video_buffer.resize(WIDTH * HEIGHT);
    }

    void screen_display::clear()
    {
        SDL_memset(std::data(m_video_buffer), 0, WIDTH * HEIGHT);
    }

    void screen_display::update_frame() noexcept
    {
        SDL_UpdateTexture(m_texture.get(), nullptr, std::data(m_video_buffer), PITCH);
    }

    void screen_display::render_frame() noexcept
    {
        SDL_RenderCopy(m_renderer.get(), m_texture.get(), nullptr, &m_render_target);
    }

    [[nodiscard]] u32 &screen_display::pixel(u64 pos) noexcept
    {
        return m_video_buffer[pos];
    }
}
