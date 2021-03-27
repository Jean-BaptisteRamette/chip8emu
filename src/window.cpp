#include <chip8emu/emulator/screen_display.hpp>
#include <chip8emu/window.hpp>
#include <iostream>

#include <glad/glad.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>


static constexpr auto GLSL_VERSION { "#version 150" };

window::window() :
        m_handle( SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOW_SHOWN,
                               emu::screen_display::SCALED_WIDTH + 400,
                               emu::screen_display::SCALED_HEIGHT + 400,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL), SDLWindowDestroyer() ),

        m_renderer( SDL_CreateRenderer(m_handle.get(), -1, SDL_RENDERER_ACCELERATED), SDLRendererDestroyer() ),
        m_chip8emu(m_renderer)
{
    if (!m_handle || !m_renderer)
        throw std::runtime_error("[ERROR]: Could not initialize SDL window and renderer");

    init_rendering_context();

    const emu::cartridge cart("../roms/BRIX");
    m_chip8emu.insert_cartridge(cart);
}

window::~window() noexcept
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(m_gl_context);

    SDL_Quit();
}

void window::init_rendering_context()
{
    /* Loads OpenGL in order to use ImGui with SDL2 */
    m_gl_context = SDL_GL_CreateContext(m_handle.get());

    if (!m_gl_context)
        throw std::runtime_error("Could not create an OpenGL context with SDL, aborting");

    SDL_GL_MakeCurrent(m_handle.get(), m_gl_context);

    /* Load OpenGL with glad */
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        throw std::runtime_error("Could not load OpenGL with GLAD, ImGui cannot run, aborting");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL2_InitForOpenGL(m_handle.get(), m_gl_context);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

void window::mainloop()
{
    bool loop { true };

    while (loop)
    {
        SDL_Event event;
        handle_events(&event, loop);

        m_chip8emu.execute_cpu_cycle();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_handle.get());
        ImGui::NewFrame();

        m_debugger_ui.update();
        /* TODO: only render when draw flag is set (when DXYN opcode has been executed) */
        render();
    }
}

void window::handle_events(SDL_Event *event, bool& keep_looping)
{
    while (SDL_PollEvent(event))
    {
        ImGui_ImplSDL2_ProcessEvent(event);
        m_chip8emu.handle_physical_event(event);

        switch (event->type)
        {
            case SDL_QUIT:
                keep_looping = false;
                break;
                /* Handle window events */
        }
    }
}

void window::render()
{
    /* Render ImGui windows and the emulator display */
    ImGui::Render();
    SDL_RenderPresent(m_renderer.get());

    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

[[maybe_unused]] void window::set_title(const char *title) const noexcept
{
    SDL_SetWindowTitle(m_handle.get(), title);
}
