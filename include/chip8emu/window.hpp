#ifndef CHIP8_EMU_WINDOW_HPP
#define CHIP8_EMU_WINDOW_HPP

#include <memory>
#include <chip8emu/emulator/chip8.hpp>
#include <SDL2/SDL.h>


/*
 * This class is not intended to be the CHIP8 window.
 * The actual CHI8 display is managed by the screen_display class
 *
 * This window class is just a wrapper around SDL_Window on which we display
 * the main UI and the screen_display's video buffer.
 */
class window
{
    /* custom deleter for unique_ptr */
    struct SDLWindowDestroyer
    {
        void operator()(SDL_Window* r) const noexcept { SDL_DestroyWindow(r); }
    };

    struct SDLRendererDestroyer
    {
        void operator()(SDL_Renderer* r) const noexcept { SDL_DestroyRenderer(r); }
    };

public:
    window();
    ~window() noexcept = default;
    window(const window &) = delete;
    window(window &&) = delete;
    window &operator=(const window &) = delete;
    window &operator=(window &&) = delete;

public:
    void mainloop();
    void render();

    [[maybe_unused]] void set_title(const char* title) const noexcept;

private:
    std::unique_ptr<SDL_Window, SDLWindowDestroyer> m_handle;
    std::shared_ptr<SDL_Renderer> m_renderer;

    emulator::chip8 chip8emu;
    // other window components such as the debugger
};

#endif //CHIP8_EMU_WINDOW_HPP
