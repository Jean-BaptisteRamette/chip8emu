#include <chip8emu/emulator/chip8.hpp>
#include <SDL2/SDL.h>


namespace emu
{
chip8::chip8(const std::shared_ptr<SDL_Renderer>& renderer_ptr) :
    dev_bus(renderer_ptr)
{
    dev_bus.apu.mute(true);
}

void chip8::insert_cartridge(const cartridge& cartridge)
{
    dev_bus.ram.load_machine_code(cartridge.binary);
}

[[maybe_unused]] void chip8::remove_cartridge()
{
    /* Reset all components */
}

void chip8::handle_physical_event(SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_KEYDOWN: dev_bus.keypad.update_state(event->key.keysym.sym, true);  break;
        case SDL_KEYUP:   dev_bus.keypad.update_state(event->key.keysym.sym, false); break;
    }
}

void chip8::execute_cpu_cycle()
{
    dev_bus.cpu.fetch_instruction();
    dev_bus.cpu.exec_instruction();
    dev_bus.screen.render_frame();

    if (dev_bus.cpu.timer() > 0)
        dev_bus.cpu.tick();

    if (dev_bus.apu.timer() > 0)
    {
        dev_bus.apu.tick();

        if (!dev_bus.apu.muted())
            dev_bus.apu.beep(700, 40); // TODO: Make this user modifiable
    }
}
}
