#include <chip8emu/emulator/chip8.hpp>
#include <SDL2/SDL.h>


namespace emulator
{

void chip8::insert_cartridge(const cartridge& cartridge)
{
    m_bus.ram.load_machine_code(cartridge.binary);
}

[[maybe_unused]] void chip8::remove_cartridge()
{
    /* Reset all components */
}

void chip8::run()
{
    bool loop { true };

    while (loop)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT: loop = false; break;
                case SDL_KEYDOWN: m_bus.keypad.update_state(event.key.keysym.sym, false); break;
                case SDL_KEYUP:   m_bus.keypad.update_state(event.key.keysym.sym, true);  break;
            }
        }

        m_bus.cpu.fetch_instruction();
        m_bus.cpu.exec_instruction();
    }
}

}
