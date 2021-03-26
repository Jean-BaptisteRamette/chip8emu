#include <iostream>
#include <chip8emu/window.hpp>

int main()
{
    /*
     * Later, the command line argument will only be used for the assembler
     * The ROM file input is done graphically
     */

    try
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)
        {
            std::cerr << "[ERROR]: Could not initialize SDL subsystems [VIDEO|AUDIO]\n";
            std::cerr << SDL_GetError() << std::endl;

            return EXIT_FAILURE;
        }

        window win;
        win.mainloop();

        SDL_Quit();

    } catch (const std::runtime_error& err)
    {
        SDL_Quit();

        std::cerr << "[ERROR]: " << err.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
