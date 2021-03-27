#ifndef CHIP8_EMU_DEBUGGER_HPP
#define CHIP8_EMU_DEBUGGER_HPP


namespace dbg
{
    class debugger final
    {
    public:
        debugger() noexcept = default;
        ~debugger() noexcept = default;
        debugger(const debugger&) = delete;
        debugger(debugger&&) = delete;
        debugger& operator=(const debugger&) = delete;
        debugger& operator=(debugger&&) = delete;

    private:
    };
}

#endif //CHIP8_EMU_DEBUGGER_HPP
