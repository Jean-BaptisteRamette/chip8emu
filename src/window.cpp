#include <chip8emu/emulator/screen_display.hpp>
#include <chip8emu/window.hpp>

#include <glad/glad.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

#ifdef __linux__
    static constexpr auto GLSL_VERSION { "#version 150" };
#elif _WIN32
    static constexpr auto GLSL_VERSION { "#version 130" };
#elif __APPLE__
    // required on MacOS
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    static constexpr auto GLSL_VERSION { "#version 150" };
#endif


window::window() :
        m_handle( SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               emu::screen_display::SCALED_WIDTH  + csl::console::WIDTH,
                               emu::screen_display::SCALED_HEIGHT + dbg::dbg_interface::HEIGHT,
                               SDL_WINDOW_OPENGL), SDLWindowDestroyer() ),

        m_renderer( SDL_CreateRenderer(m_handle.get(), -1, SDL_RENDERER_ACCELERATED), SDLRendererDestroyer() ),
        m_chip8emu(m_renderer)
{
    if (!m_handle || !m_renderer)
        throw std::runtime_error("Could not initialize SDL window and renderer");

    init_rendering_context();

    /* Try to load a game */
    const emu::cartridge game_cart("../roms/BRIX");
    m_chip8emu.insert_cartridge(game_cart);
    m_chip8emu.dev_bus.apu.mute();

    /* Start the debugger with the loaded program */
    m_debugger_ui.attach_process_handle(m_chip8emu);
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
    /* Loads OpenGL and inits ImGui SDL2 implementation */
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
        handle_events(event, loop);

        if (!m_chip8emu.dev_bus.cpu.is_flag_set(emu::proc_flags::HALT))
        {
            try
            {
                m_chip8emu.execute_cpu_cycle();
            } catch (const emu::opcode_decoding_error& err)
            {
                show_error(err);
            } catch (const emu::memory_access_error& err)
            {
                show_error(err);
            }
        }

        /*
         * FIXME: We should only  call update_frame when the draw flag is set
         *        Meaning that a draw instruction has been executed and we
         *        should update the emulator display.
         *
         * FIXME: But when we do that, the ImGui windows don't render, because
         *        SDL_UpdateTexture() isn't called, but this function is not
         *        supposed to interfere with ImGui's rendering.
         *
         * You can see that if line ... is commented, the ImGui windows won't render
         *
         * See screen_display::update_frame function in screen_display.cpp
         */

        /*
        if (m_chip8emu.dev_bus.cpu.is_flag_set(emu::proc_flags::DRAW))
        {
            m_chip8emu.dev_bus.screen.update_frame();
            m_chip8emu.dev_bus.cpu.set_flag(emu::proc_flags::DRAW, false);
        }
        */

        m_chip8emu.dev_bus.screen.update_frame();
        m_chip8emu.dev_bus.screen.render_frame();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_handle.get());
        ImGui::NewFrame();

        m_console_ui.update();
        m_debugger_ui.update();

        // Perhaps we should just use flags instead of exceptions
        // show_error(decode_error);
        // show_error(memory_error);

        render();
    }
}

void window::handle_events(SDL_Event& event, bool& keep_looping)
{
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        m_chip8emu.handle_physical_event(event);

        if (event.type == SDL_QUIT)
            keep_looping = false;
    }
}

void window::render()
{
    /* Render ImGui windows and the m_emulator display */
    ImGui::Render();
    SDL_RenderPresent(m_renderer.get());
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void window::set_title(const char *title) const noexcept
{
    SDL_SetWindowTitle(m_handle.get(), title);
}

void window::show_error(const emu::opcode_decoding_error& error) const
{
    if (ImGui::BeginPopupModal("Opcode Error"))
    {
        ImGui::Text("%s: %hu.", error.what(), error.opcode());
        ImGui::Separator();

        if (ImGui::Button("Ok", ImVec2(120, 0)))
            ImGui::CloseCurrentPopup();

        ImGui::SetItemDefaultFocus();

        ImGui::EndPopup();
    }
}

void window::show_error(const emu::memory_access_error& error) const
{
    if (ImGui::BeginPopupModal("Memory Access Error"))
    {
        ImGui::Text("%s at %hu.", error.what(), error.address());
        ImGui::Separator();

        if (ImGui::Button("Ok", ImVec2(120, 0)))
            ImGui::CloseCurrentPopup();

        ImGui::SetItemDefaultFocus();

        ImGui::EndPopup();
    }
}
