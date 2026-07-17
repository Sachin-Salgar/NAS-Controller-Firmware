#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Config
 * File    : ProtocolConfig.h
 *
 ******************************************************************************/

#include <cstddef>
#include <cstdint>

namespace NAS::Config::Protocol
{

inline constexpr std::uint16_t PacketHeader = 0x55AA;

inline constexpr std::size_t MaximumPacketSize = 512U;

inline constexpr std::size_t MaximumPayloadSize = 500U;

inline constexpr std::size_t UsbReceiveBufferSize = 512U;

inline constexpr std::size_t UsbTransmitBufferSize = 512U;

inline constexpr std::uint16_t ProtocolVersion = 0x0100;

} // namespace NAS::Config::Protocol