#ifndef CHIP8_EMU_AUDIO_UNIT_HPP
#define CHIP8_EMU_AUDIO_UNIT_HPP

#include <chip8emu/inttypes.hpp>
#include <queue>


namespace emulator
{
    class audio_unit final
    {
        struct beep_sound
        {
            double frequency;
            int samples_left;
        };

        static constexpr u16 FREQ { 44100 };
        static constexpr u16 AMPLITUDE_FACTOR { 28000 };

    public:
        audio_unit();
        ~audio_unit() noexcept;
        audio_unit(const audio_unit &) = delete;
        audio_unit(audio_unit &&) = delete;
        audio_unit &operator=(const audio_unit &) = delete;
        audio_unit &operator=(audio_unit &&) = delete;

        void beep(double freq, int duration);

        /* decrement the timer */
        void tick() noexcept;

        void mute(bool value) noexcept;
        [[nodiscard]] bool muted() const noexcept;

        void set_timer(u8 value) noexcept;
        [[nodiscard]] u8 timer() const noexcept;

        /* Make a device_opened function as we may want to keep running even if the sound wasn't initialized */

        void build_samples(s16* stream, int len);

    private:
        std::queue<beep_sound> m_audio_queue;
        u8 m_timer { 0 };

        /*
         * Variable which just change the value given to
         * the sine function, in order to oscillate the
         * sine wave between -AMPLITUDE_FACTOR and AMPLITUDE_FACTOR
         */
        double wave_state { 0 };

        bool m_muted {};
    };
}


#endif //CHIP8_EMU_AUDIO_UNIT_HPP
