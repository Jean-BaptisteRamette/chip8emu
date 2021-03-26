#include <chip8emu/emulator/audio_unit.hpp>
#include <SDL2/SDL_audio.h>
#include <numbers>
#include <stdexcept>


namespace emulator
{

    /* TODO: Use SDL_AudioDevice */
static void audio_callback(void* userdata, u8* stream, int len)
{
    s16* stream16 { (s16*)stream };
    auto apu { static_cast<audio_unit*>(userdata) };
    apu->build_samples(stream16, len / 2);
}

audio_unit::audio_unit()
{
    /*
     * There is no specifications on what sound the CHIP8 is supposed to produce.
     * We are totally free for this part
     */

    /* Open Audio Device */
    SDL_AudioSpec desired;
    SDL_AudioSpec obtained;

    desired.freq = FREQ;
    desired.format = AUDIO_S16SYS;
    desired.channels = 1;
    desired.samples = FREQ / 20;

    desired.userdata = this;
    desired.callback = audio_callback;

    if (SDL_OpenAudio(&desired, &obtained) != 0)
        throw std::runtime_error("Could not find an audio device satisfying the specification");

    /* Unpause audio device */
    SDL_PauseAudio(0);
}

audio_unit::~audio_unit() noexcept
{
    SDL_CloseAudio();
}

void audio_unit::build_samples(s16* stream, int len)
{
    /* Modified the implementation found here found here: https://web.archive.org/web/20120313055436/http://www.dgames.org/beep-sound-with-sdl/ */

    /* Generates sine wave_state samples */
    int i { 0 };

    while (i < len)
    {
        if (m_audio_queue.empty())
        {
            /* No sound should be played */
            while (i < len)
            {
                stream[i] = 0;
                ++i;
            }
            return;
        }

        auto& beep_s { m_audio_queue.front() };

        const int samples_todo { std::min(i + beep_s.samples_left, len) };
        beep_s.samples_left -= samples_todo - i;

        while (i < samples_todo)
        {
            stream[i] = AMPLITUDE_FACTOR * std::sin(wave_state * 2 * std::numbers::pi / FREQ);
            ++i;
            wave_state += beep_s.frequency;
        }

        if (beep_s.samples_left == 0)
            m_audio_queue.pop();
    }
}

void audio_unit::beep(double freq, int duration)
{
    /* beep with at a certain frequency and duration */
    SDL_LockAudio();
    m_audio_queue.emplace(freq, duration * FREQ / 1000);
    SDL_UnlockAudio();
}

void audio_unit::tick() noexcept
{
    --m_timer;
}

void audio_unit::set_timer(u8 value) noexcept
{
    m_timer = value;
}

[[nodiscard]] u8 audio_unit::timer() const noexcept
{
    return m_timer;
}

void audio_unit::mute(bool value) noexcept
{
    m_muted = value;
}

[[nodiscard]] bool audio_unit::muted() const noexcept
{
    return m_muted;
}
}