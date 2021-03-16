#ifndef CHIP8_EMU_MEMORY_HPP
#define CHIP8_EMU_MEMORY_HPP

#include <array>
#include <algorithm>
#include <chip8emu/inttypes.hpp>


namespace emulator
{
    using address_type = u16;

    /* useful for the debugger */
    class memory_access_error : public std::runtime_error
    {
    public:
        explicit memory_access_error(address_type address)
                : std::runtime_error("Memory Access Violation"),
                  m_addr(address)
        {};

        [[nodiscard]] address_type address() const noexcept { return m_addr; };

    private:
        address_type m_addr;
    };

    class memory final
    {
        /*!
         * Chip8 memory has a range of 4095 bytes.
         * The machine code must be loaded at address 512.
         * The memory also contains a font already at address 80.
         */

    public:
        static constexpr address_type SIZE  {0xFFF };
        static constexpr address_type FONT_ADDRESS { 0x50  };
        static constexpr address_type CODE_ADDRESS { 0x200 };
        static constexpr address_type CODE_MAXSIZE { SIZE - CODE_ADDRESS };

    public:
        memory();
        ~memory() noexcept = default;
        memory(const memory &) = delete;
        memory(memory &&) = delete;
        memory &operator=(const memory &) = delete;
        memory &operator=(memory &&) = delete;

        /* read and write operations */
              u8 &operator[](address_type address);
        const u8 &operator[](address_type address) const;

        void load_machine_code(const std::vector<u8> &binary);

    private:
        std::array<u8, SIZE> m_ram;
    };
}


#endif //CHIP8_EMU_MEMORY_HPP
