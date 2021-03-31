#ifndef CHIP8_EMU_SCREEN_DISPLAY_HPP
#define CHIP8_EMU_SCREEN_DISPLAY_HPP

#include <chip8emu/inttypes.hpp>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>


namespace emu
{
    class screen_display final
    {
        struct SDLTextureDestroyer
        {
            void operator()(SDL_Texture* t) const noexcept { SDL_DestroyTexture(t); }
        };

    public:
        static constexpr u8 WIDTH { 64 };
        static constexpr u8 HEIGHT { 32 };
        static constexpr u8 PIXEL_SCALE { 15 };  // one pixel is actually PIXEL_SCALE times bigger
        static constexpr u32 SCALED_WIDTH { WIDTH * PIXEL_SCALE };
        static constexpr u32 SCALED_HEIGHT { HEIGHT * PIXEL_SCALE };
        static constexpr u32 PITCH { sizeof(u32) * WIDTH };

    public:
        explicit screen_display(const std::shared_ptr<SDL_Renderer>& renderer);
        ~screen_display() noexcept = default;
        screen_display(const screen_display &) = delete;
        screen_display(screen_display &&) = delete;
        screen_display &operator=(const screen_display &) = delete;
        screen_display &operator=(screen_display &&) = delete;

        /* clear video buffer */
        void clear();

        /* copy video buffer to texture and display texture */
        void render_frame() noexcept;

        /* get mutable pixel from video buffer */
        [[nodiscard]] u32& pixel(u64 pos) noexcept;

        /* allow user to resize render target */

    private:
        std::shared_ptr<SDL_Renderer> m_renderer;
        std::unique_ptr<SDL_Texture, SDLTextureDestroyer> m_texture;

        /* Area where the emu renders in the window */
        SDL_Rect m_render_target { 0, 0, SCALED_WIDTH, SCALED_HEIGHT };
        std::vector<u32> m_video_buffer;
    };
}

#endif //CHIP8_EMU_SCREEN_DISPLAY_HPP
