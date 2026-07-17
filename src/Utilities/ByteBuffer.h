#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Utilities
 * File    : ByteBuffer.h
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

namespace NAS::Utilities
{

class ByteBuffer
{
public:

    ByteBuffer(
        std::uint8_t* buffer,
        std::size_t capacity) noexcept;

    [[nodiscard]]
    std::size_t Size() const noexcept;

    [[nodiscard]]
    std::size_t Capacity() const noexcept;

    [[nodiscard]]
    std::size_t Remaining() const noexcept;

    [[nodiscard]]
    bool IsEmpty() const noexcept;

    [[nodiscard]]
    bool IsFull() const noexcept;

    void Clear() noexcept;

    [[nodiscard]]
    bool WriteUInt8(
        std::uint8_t value) noexcept;

    [[nodiscard]]
    bool WriteUInt16(
        std::uint16_t value) noexcept;

    [[nodiscard]]
    bool WriteUInt32(
        std::uint32_t value) noexcept;

    [[nodiscard]]
    bool WriteBytes(
        const std::uint8_t* data,
        std::size_t length) noexcept;

    [[nodiscard]]
    bool ReadUInt8(
        std::size_t offset,
        std::uint8_t& value) const noexcept;

    [[nodiscard]]
    bool ReadUInt16(
        std::size_t offset,
        std::uint16_t& value) const noexcept;

    [[nodiscard]]
    bool ReadUInt32(
        std::size_t offset,
        std::uint32_t& value) const noexcept;

    [[nodiscard]]
    const std::uint8_t* Data() const noexcept;

    [[nodiscard]]
    std::uint8_t* Data() noexcept;

private:

    std::uint8_t* buffer_;

    std::size_t capacity_;

    std::size_t size_;
};

} // namespace NAS::Utilities