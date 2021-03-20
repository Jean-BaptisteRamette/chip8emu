#include <chip8emu/window.hpp>
#include <chip8emu/emulator/screen_display.hpp>


window::window() :
    m_handle( SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOW_SHOWN,
                               emulator::screen_display::SCALED_WIDTH + 300,
                               emulator::screen_display::SCALED_HEIGHT + 100,
                               SDL_WINDOW_SHOWN), SDLWindowDestroyer() ),

    m_renderer( SDL_CreateRenderer(m_handle.get(), -1, SDL_RENDERER_ACCELERATED), SDLRendererDestroyer() ),
    chip8emu(m_renderer)
{
    if (!m_handle || !m_renderer)
        throw std::runtime_error("[ERROR]: Could not initialize SDL window and renderer");

    /* TODO: when working with ImGui, load file with GUI text input */
    const emulator::cartridge cart("../roms/BRIX");
    chip8emu.insert_cartridge(cart);
}

void window::mainloop()
{
    bool loop { true };

    while (loop)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            chip8emu.handle_physical_event(&event);

            switch (event.type)
            {
                case SDL_QUIT: loop = false; break;
                /* Handle window events */
            }
        }

        chip8emu.execute_cpu_cycle();

        SDL_Delay(2);

        render();
    }
}

void window::render()
{
    SDL_RenderPresent(m_renderer.get());
}

[[maybe_unused]] void window::set_title(const char *title) const noexcept
{
    SDL_SetWindowTitle(m_handle.get(), title);
}
