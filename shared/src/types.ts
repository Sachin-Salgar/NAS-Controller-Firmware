// ============================================================================
// PURE TYPE ALIASES AND SIMPLE DATA TYPES
// ============================================================================

// Numeric type aliases for clarity
export type Uint8 = number;
export type Uint16 = number;
export type Uint32 = number;
export type Int8 = number;
export type Int16 = number;

// Common value ranges
export type RelayId = number;
export type FanId = number;
export type SensorId = number;
export type DriveId = number;
export type LEDId = number;
export type SequenceNumber = number;
export type Timestamp = number;
export type PercentageValue = number; // 0-100
export type Temperature = number; // in Celsius, multiplied by 100

// Command and response metadata
export interface CommandMetadata {
  sequence: SequenceNumber;
  timestamp: Timestamp;
  attempts: number;
  success: boolean;
  duration: number;
}

export interface ResponseMetadata {
  sequence: SequenceNumber;
  timestamp: Timestamp;
  wasAck: boolean;
  errorCode?: number;
}
