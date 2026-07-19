/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Utilities
 * File    : ByteBuffer.cpp
 *
 ******************************************************************************/

#include "ByteBuffer.h"

#include <cstring>

namespace NAS::Utilities
{

ByteBuffer::ByteBuffer(
    std::uint8_t* buffer,
    std::size_t capacity) noexcept
    : buffer_(buffer),
      capacity_(capacity),
      size_(0U)
{
}

std::size_t ByteBuffer::Size() const noexcept
{
    return size_;
}

std::size_t ByteBuffer::Capacity() const noexcept
{
    return capacity_;
}

std::size_t ByteBuffer::Remaining() const noexcept
{
    return capacity_ - size_;
}

bool ByteBuffer::IsEmpty() const noexcept
{
    return size_ == 0U;
}

bool ByteBuffer::IsFull() const noexcept
{
    return size_ >= capacity_;
}

void ByteBuffer::Clear() noexcept
{
    size_ = 0U;
}

bool ByteBuffer::WriteUInt8(
    std::uint8_t value) noexcept
{
    return WriteBytes(&value, sizeof(value));
}

bool ByteBuffer::WriteUInt16(
    std::uint16_t value) noexcept
{
    return WriteBytes(
        reinterpret_cast<const std::uint8_t*>(&value),
        sizeof(value));
}

bool ByteBuffer::WriteUInt32(
    std::uint32_t value) noexcept
{
    return WriteBytes(
        reinterpret_cast<const std::uint8_t*>(&value),
        sizeof(value));
}

bool ByteBuffer::WriteBytes(
    const std::uint8_t* data,
    std::size_t length) noexcept
{
    if ((data == nullptr) ||
        (size_ + length > capacity_))
    {
        return false;
    }

    std::memcpy(
        buffer_ + size_,
        data,
        length);

    size_ += length;

    return true;
}

bool ByteBuffer::ReadUInt8(
    std::size_t offset,
    std::uint8_t& value) const noexcept
{
    if (offset + sizeof(value) > size_)
    {
        return false;
    }

    value = buffer_[offset];

    return true;
}

bool ByteBuffer::ReadUInt16(
    std::size_t offset,
    std::uint16_t& value) const noexcept
{
    if (offset + sizeof(value) > size_)
    {
        return false;
    }

    std::memcpy(
        &value,
        buffer_ + offset,
        sizeof(value));

    return true;
}

bool ByteBuffer::ReadUInt32(
    std::size_t offset,
    std::uint32_t& value) const noexcept
{
    if (offset + sizeof(value) > size_)
    {
        return false;
    }

    std::memcpy(
        &value,
        buffer_ + offset,
        sizeof(value));

    return true;
}

const std::uint8_t* ByteBuffer::Data() const noexcept
{
    return buffer_;
}

std::uint8_t* ByteBuffer::Data() noexcept
{
    return buffer_;
}

} // namespace NAS::Utilities