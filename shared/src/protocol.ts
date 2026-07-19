// ============================================================================
// PROTOCOL INFORMATION
// ============================================================================
// Consolidated protocol metadata - single source of truth for protocol details

export const ProtocolInfo = {
  Version: "1.0.0",
  ConfigVersion: 1,
  Header: 0x55aa,
  Footer: 0xaa,
  CRC: "CRC16-CCITT",
  CRCInitialValue: 0xffff,
  CRCPolynomial: 0x1021,
  CRCFinalXOR: 0x0000,
  MaxPacketSize: 256,
  MaxPayloadSize: 256,
  ACKTimeout: 100,
  ResponseTimeout: 500,
  CommandTimeout: 1000,
  MaxRetryAttempts: 3,
} as const;

// ============================================================================
// RESPONSE CODE BITMASKS
// ============================================================================

export const ResponseBit = {
  ACK: 0x80,      // Bit 7: Acknowledgement
  NAK: 0x40,      // Bit 6: Negative acknowledgement
  Response: 0x90, // Bit 7,4: Data response
} as const;

// ============================================================================
// COMMAND CODES
// ============================================================================

export const CommandCode = {
  // System Commands (0x01-0x0F)
  SYSTEM_PING: 0x01,
  SYSTEM_RESET: 0x02,
  GET_CAPABILITIES: 0x03,

  // Relay Commands (0x10-0x1F)
  RELAY_SET: 0x10,
  RELAY_GET: 0x11,

  // Status Commands (0x20-0x2F)
  GET_ALL_STATUS: 0x20,

  // Fan Commands (0x30-0x3F)
  FAN_SET_SPEED: 0x30,
  FAN_GET: 0x31,
  FAN_SET_MODE: 0x32,

  // Temperature Commands (0x40-0x4F)
  TEMP_READ: 0x40,

  // LED Commands (0x50-0x5F)
  LED_SET_COLOR: 0x50,
  LED_SET_BRIGHTNESS: 0x51,
  LED_SET_ANIMATION: 0x52,

  // Configuration Commands (0x60-0x6F)
  CONFIG_GET: 0x60,
  CONFIG_SET: 0x61,
  CONFIG_BEGIN: 0x62,
  CONFIG_COMMIT: 0x63,

  // Event Log Commands (0x70-0x7F)
  GET_EVENT_LOG: 0x70,
} as const;

// ============================================================================
// ENUMERATIONS
// ============================================================================

export enum ErrorCode {
  OK = 0x00,
  INVALID_COMMAND = 0x01,
  INVALID_PAYLOAD = 0x02,
  INVALID_SEQUENCE = 0x03,
  CRC_FAILED = 0x04,
  DEVICE_NOT_READY = 0x05,
  TIMEOUT = 0x06,
  OUT_OF_RANGE = 0x07,
  CONFIG_LOCKED = 0x08,
  CONFIG_INVALID = 0x09,
  UNKNOWN_ERROR = 0x0a,
}

export enum RelayState {
  OFF = 0x00,
  ON = 0x01,
}

export enum FanMode {
  MANUAL = 0x00,
  AUTO = 0x01,
  OFF = 0x02,
}

export enum LEDAnimation {
  BOOT = 0x00,
  IDLE = 0x01,
  ERROR = 0x02,
  SHUTDOWN = 0x03,
  CUSTOM = 0x04,
}

// ============================================================================
// PACKET STRUCTURE DEFINITIONS
// ============================================================================

export interface PacketHeader {
  magic: number; // 0x55AA (2 bytes, big-endian)
}

export interface PacketMetadata {
  sequence: number;      // 1 byte (0x00-0xFF)
  command: number;       // 1 byte
  length: number;        // 2 bytes (payload length, big-endian)
}

export interface PacketFooter {
  crc16: number;  // 2 bytes (big-endian)
  terminator: number; // 1 byte (0xAA)
}

export interface Packet {
  header: PacketHeader;
  metadata: PacketMetadata;
  payload: Uint8Array;
  footer: PacketFooter;
}

export interface ParsedPacket {
  sequence: number;
  command: number;
  payload: Uint8Array;
}

// ============================================================================
// DATA TRANSFER OBJECTS (DTOs)
// ============================================================================

// Capability definitions
export interface CapabilityInfo {
  protocolVersion: number;
  configVersion: number;
  firmwareVersion: number;
  relayCount: number;
  fanCount: number;
  ledCount: number;
  temperatureSensorCount: number;
  driveCount: number;
  supportedCommands: Uint8Array;
}

// Hardware configuration DTOs
export interface RelayConfig {
  id: number;
  label?: string;
}

export interface FanConfig {
  id: number;
  label?: string;
  minSpeed: number;
  maxSpeed: number;
}

export interface TemperatureSensorConfig {
  id: number;
  label?: string;
}

export interface DriveConfig {
  id: number;
  label?: string;
}

export interface LEDConfig {
  id: number;
  label?: string;
}

// Hardware status DTOs
export interface RelayStatus {
  id: number;
  state: RelayState;
}

export interface FanStatus {
  id: number;
  speed: number;
  mode: FanMode;
  status: number;
}

export interface TemperatureStatus {
  id: number;
  temperature: number;
}

export interface DriveStatus {
  id: number;
  present: boolean;
}

export interface LEDStatus {
  id: number;
  red: number;
  green: number;
  blue: number;
  brightness: number;
  animation: LEDAnimation;
}

export interface SystemStatus {
  relays: RelayStatus[];
  fans: FanStatus[];
  temperatures: TemperatureStatus[];
  drives: DriveStatus[];
  leds: LEDStatus[];
  timestamp: number;
}

// Configuration DTOs
export interface ConfigurationData {
  version: number;
  fanAutoMinSpeed: number;
  fanAutoMaxSpeed: number;
  tempWarningThreshold: number;
  tempCriticalThreshold: number;
  autoStartEnabled: boolean;
  debugMode: boolean;
}

// Event log DTOs
export interface EventLogEntry {
  index: number;
  timestamp: number;
  eventType: number;
  data: Uint8Array;
}

// ============================================================================
// COMMAND REQUEST TYPES
// ============================================================================

export interface BaseCommand {
  sequence: number;
  command: number;
}

export interface SystemPingRequest extends BaseCommand {
  command: typeof CommandCode.SYSTEM_PING;
}

export interface SystemResetRequest extends BaseCommand {
  command: typeof CommandCode.SYSTEM_RESET;
}

export interface GetCapabilitiesRequest extends BaseCommand {
  command: typeof CommandCode.GET_CAPABILITIES;
}

export interface RelaySetRequest extends BaseCommand {
  command: typeof CommandCode.RELAY_SET;
  relayId: number;
  state: RelayState;
}

export interface RelayGetRequest extends BaseCommand {
  command: typeof CommandCode.RELAY_GET;
  relayId: number;
}

export interface FanSetSpeedRequest extends BaseCommand {
  command: typeof CommandCode.FAN_SET_SPEED;
  fanId: number;
  speed: number;
}

export interface FanGetRequest extends BaseCommand {
  command: typeof CommandCode.FAN_GET;
  fanId: number;
}

export interface FanSetModeRequest extends BaseCommand {
  command: typeof CommandCode.FAN_SET_MODE;
  fanId: number;
  mode: FanMode;
}

export interface TemperatureReadRequest extends BaseCommand {
  command: typeof CommandCode.TEMP_READ;
  sensorId: number;
}

export interface LEDSetColorRequest extends BaseCommand {
  command: typeof CommandCode.LED_SET_COLOR;
  red: number;
  green: number;
  blue: number;
}

export interface LEDSetBrightnessRequest extends BaseCommand {
  command: typeof CommandCode.LED_SET_BRIGHTNESS;
  brightness: number;
}

export interface LEDSetAnimationRequest extends BaseCommand {
  command: typeof CommandCode.LED_SET_ANIMATION;
  animation: LEDAnimation;
}

export interface ConfigGetRequest extends BaseCommand {
  command: typeof CommandCode.CONFIG_GET;
}

export interface ConfigSetRequest extends BaseCommand {
  command: typeof CommandCode.CONFIG_SET;
  configData: Uint8Array;
}

export interface ConfigBeginRequest extends BaseCommand {
  command: typeof CommandCode.CONFIG_BEGIN;
}

export interface ConfigCommitRequest extends BaseCommand {
  command: typeof CommandCode.CONFIG_COMMIT;
}

export interface GetAllStatusRequest extends BaseCommand {
  command: typeof CommandCode.GET_ALL_STATUS;
}

export interface GetEventLogRequest extends BaseCommand {
  command: typeof CommandCode.GET_EVENT_LOG;
  startIndex: number;
  count: number;
}

// Union of all command types
export type Command =
  | SystemPingRequest
  | SystemResetRequest
  | GetCapabilitiesRequest
  | RelaySetRequest
  | RelayGetRequest
  | FanSetSpeedRequest
  | FanGetRequest
  | FanSetModeRequest
  | TemperatureReadRequest
  | LEDSetColorRequest
  | LEDSetBrightnessRequest
  | LEDSetAnimationRequest
  | ConfigGetRequest
  | ConfigSetRequest
  | ConfigBeginRequest
  | ConfigCommitRequest
  | GetAllStatusRequest
  | GetEventLogRequest;

// ============================================================================
// RESPONSE TYPES
// ============================================================================

export interface BaseResponse {
  sequence: number;
  command: number;
}

export interface AckResponse extends BaseResponse {
  isAck: true;
  isNak: false;
  errorCode?: never;
  payload?: never;
}

export interface NakResponse extends BaseResponse {
  isAck: false;
  isNak: true;
  errorCode: ErrorCode;
  payload?: never;
}

export interface DataResponse extends BaseResponse {
  isAck: false;
  isNak: false;
  errorCode?: never;
  payload: Uint8Array;
}

export type Response = AckResponse | NakResponse | DataResponse;

// ============================================================================
// EVENT DEFINITIONS
// ============================================================================

export enum EventType {
  DEVICE_CONNECTED = "device_connected",
  DEVICE_DISCONNECTED = "device_disconnected",
  RELAY_STATE_CHANGED = "relay_state_changed",
  FAN_SPEED_CHANGED = "fan_speed_changed",
  FAN_MODE_CHANGED = "fan_mode_changed",
  LED_COLOR_CHANGED = "led_color_changed",
  LED_ANIMATION_CHANGED = "led_animation_changed",
  TEMPERATURE_CHANGED = "temperature_changed",
  CONFIGURATION_CHANGED = "configuration_changed",
  COMMAND_SENT = "command_sent",
  COMMAND_COMPLETED = "command_completed",
  COMMAND_FAILED = "command_failed",
  ERROR_OCCURRED = "error_occurred",
}

export interface DeviceConnectedEvent {
  type: EventType.DEVICE_CONNECTED;
  timestamp: number;
  capabilities: CapabilityInfo;
}

export interface DeviceDisconnectedEvent {
  type: EventType.DEVICE_DISCONNECTED;
  timestamp: number;
  reason: string;
}

export interface RelayStateChangedEvent {
  type: EventType.RELAY_STATE_CHANGED;
  timestamp: number;
  relayId: number;
  previousState: RelayState;
  newState: RelayState;
}

export interface FanSpeedChangedEvent {
  type: EventType.FAN_SPEED_CHANGED;
  timestamp: number;
  fanId: number;
  previousSpeed: number;
  newSpeed: number;
}

export interface FanModeChangedEvent {
  type: EventType.FAN_MODE_CHANGED;
  timestamp: number;
  fanId: number;
  previousMode: FanMode;
  newMode: FanMode;
}

export interface LEDColorChangedEvent {
  type: EventType.LED_COLOR_CHANGED;
  timestamp: number;
  ledId: number;
  red: number;
  green: number;
  blue: number;
}

export interface LEDAnimationChangedEvent {
  type: EventType.LED_ANIMATION_CHANGED;
  timestamp: number;
  ledId: number;
  animation: LEDAnimation;
}

export interface TemperatureChangedEvent {
  type: EventType.TEMPERATURE_CHANGED;
  timestamp: number;
  sensorId: number;
  previousTemperature: number;
  newTemperature: number;
}

export interface ConfigurationChangedEvent {
  type: EventType.CONFIGURATION_CHANGED;
  timestamp: number;
  section: string;
}

export interface CommandSentEvent {
  type: EventType.COMMAND_SENT;
  timestamp: number;
  sequence: number;
  command: number;
}

export interface CommandCompletedEvent {
  type: EventType.COMMAND_COMPLETED;
  timestamp: number;
  sequence: number;
  command: number;
  duration: number;
}

export interface CommandFailedEvent {
  type: EventType.COMMAND_FAILED;
  timestamp: number;
  sequence: number;
  command: number;
  errorCode: ErrorCode;
  attempts: number;
}

export interface ErrorOccurredEvent {
  type: EventType.ERROR_OCCURRED;
  timestamp: number;
  errorCode: string;
  message: string;
  severity: "warning" | "error";
}

export type Event =
  | DeviceConnectedEvent
  | DeviceDisconnectedEvent
  | RelayStateChangedEvent
  | FanSpeedChangedEvent
  | FanModeChangedEvent
  | LEDColorChangedEvent
  | LEDAnimationChangedEvent
  | TemperatureChangedEvent
  | ConfigurationChangedEvent
  | CommandSentEvent
  | CommandCompletedEvent
  | CommandFailedEvent
  | ErrorOccurredEvent;

// ============================================================================
// RESULT TYPE FOR SUCCESS/FAILURE HANDLING
// ============================================================================

export type Success<T> = {
  success: true;
  value: T;
};

export type Failure = {
  success: false;
  error: {
    code: string;
    message: string;
  };
};

export type Result<T> = Success<T> | Failure;

// Constructor functions for Result
export const Ok = <T>(value: T): Result<T> => ({
  success: true,
  value,
});

export const Err = (code: string, message: string): Result<never> => ({
  success: false,
  error: { code, message },
});
