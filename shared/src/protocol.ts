// Protocol frame format constants
export const PACKET_HEADER = 0x55AA;
export const PACKET_FOOTER = 0xAA;
export const MAX_PAYLOAD_SIZE = 256;
export const PROTOCOL_VERSION = 1;

// CRC16 algorithm parameters
export const CRC16_INITIAL_VALUE = 0xFFFF;
export const CRC16_POLYNOMIAL = 0x1021;
export const CRC16_FINAL_XOR = 0x0000;

// System command codes
export const CMD_SYSTEM_PING = 0x01;
export const CMD_SYSTEM_RESET = 0x02;
export const CMD_GET_CAPABILITIES = 0x03;

// Relay command codes
export const CMD_RELAY_SET = 0x10;
export const CMD_RELAY_GET = 0x11;

// Status command codes
export const CMD_GET_ALL_STATUS = 0x20;

// Fan command codes
export const CMD_FAN_SET_SPEED = 0x30;
export const CMD_FAN_GET = 0x31;
export const CMD_FAN_SET_MODE = 0x32;

// Temperature command codes
export const CMD_TEMP_READ = 0x40;

// LED command codes
export const CMD_LED_SET_COLOR = 0x50;
export const CMD_LED_SET_BRIGHTNESS = 0x51;
export const CMD_LED_SET_ANIMATION = 0x52;

// Configuration command codes
export const CMD_CONFIG_GET = 0x60;
export const CMD_CONFIG_SET = 0x61;
export const CMD_CONFIG_BEGIN = 0x62;
export const CMD_CONFIG_COMMIT = 0x63;

// Event log command codes
export const CMD_GET_EVENT_LOG = 0x70;

// Response code bitmasks
export const ACK_MASK = 0x80;
export const NAK_MASK = 0x40;
export const RESPONSE_MASK = 0x90;

// Timeout values (milliseconds)
export const ACK_TIMEOUT_MS = 100;
export const RESPONSE_TIMEOUT_MS = 500;
export const COMMAND_TIMEOUT_MS = 1000;

// Retry configuration
export const MAX_RETRY_ATTEMPTS = 3;
export const RETRY_BACKOFF_MS = [0, 100, 200]; // Delays for retry 1, 2, 3

// Error codes
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
  UNKNOWN_ERROR = 0x0A,
}

// Relay states
export enum RelayState {
  OFF = 0x00,
  ON = 0x01,
}

// Fan modes
export enum FanMode {
  MANUAL = 0x00,
  AUTO = 0x01,
  OFF = 0x02,
}

// LED animation types
export enum LEDAnimation {
  BOOT = 0x00,
  IDLE = 0x01,
  ERROR = 0x02,
  SHUTDOWN = 0x03,
  CUSTOM = 0x04,
}

// Command state machine
export enum CommandState {
  QUEUED = "queued",
  SENDING = "sending",
  WAITING_ACK = "waiting_ack",
  COMPLETED = "completed",
  FAILED = "failed",
  TIMEOUT = "timeout",
}

// Packet structure interfaces
export interface Packet {
  header: number;
  sequence: number;
  command: number;
  length: number;
  payload: Uint8Array;
  crc16: number;
  footer: number;
}

export interface DecodedPacket {
  sequence: number;
  command: number;
  payload: Uint8Array;
}

export interface EncodedPacket {
  data: Uint8Array;
  length: number;
}

// Command request/response types
export interface BaseCommand {
  sequence: number;
  command: number;
}

// System Commands
export interface PingCommand extends BaseCommand {
  command: typeof CMD_SYSTEM_PING;
}

export interface ResetCommand extends BaseCommand {
  command: typeof CMD_SYSTEM_RESET;
}

export interface GetCapabilitiesCommand extends BaseCommand {
  command: typeof CMD_GET_CAPABILITIES;
}

// Relay Commands
export interface RelaySetCommand extends BaseCommand {
  command: typeof CMD_RELAY_SET;
  relayId: number;
  state: RelayState;
}

export interface RelayGetCommand extends BaseCommand {
  command: typeof CMD_RELAY_GET;
  relayId: number;
}

// Fan Commands
export interface FanSetSpeedCommand extends BaseCommand {
  command: typeof CMD_FAN_SET_SPEED;
  fanId: number;
  speed: number;
}

export interface FanGetCommand extends BaseCommand {
  command: typeof CMD_FAN_GET;
  fanId: number;
}

export interface FanSetModeCommand extends BaseCommand {
  command: typeof CMD_FAN_SET_MODE;
  fanId: number;
  mode: FanMode;
}

// Temperature Commands
export interface TemperatureReadCommand extends BaseCommand {
  command: typeof CMD_TEMP_READ;
  sensorId: number;
}

// LED Commands
export interface LEDSetColorCommand extends BaseCommand {
  command: typeof CMD_LED_SET_COLOR;
  red: number;
  green: number;
  blue: number;
}

export interface LEDSetBrightnessCommand extends BaseCommand {
  command: typeof CMD_LED_SET_BRIGHTNESS;
  brightness: number;
}

export interface LEDSetAnimationCommand extends BaseCommand {
  command: typeof CMD_LED_SET_ANIMATION;
  animation: LEDAnimation;
}

// Configuration Commands
export interface ConfigGetCommand extends BaseCommand {
  command: typeof CMD_CONFIG_GET;
}

export interface ConfigSetCommand extends BaseCommand {
  command: typeof CMD_CONFIG_SET;
  configData: Uint8Array;
}

export interface ConfigBeginCommand extends BaseCommand {
  command: typeof CMD_CONFIG_BEGIN;
}

export interface ConfigCommitCommand extends BaseCommand {
  command: typeof CMD_CONFIG_COMMIT;
}

// Status Commands
export interface GetAllStatusCommand extends BaseCommand {
  command: typeof CMD_GET_ALL_STATUS;
}

// Event Log Commands
export interface GetEventLogCommand extends BaseCommand {
  command: typeof CMD_GET_EVENT_LOG;
  startIndex: number;
  count: number;
}

// Union type for all commands
export type Command =
  | PingCommand
  | ResetCommand
  | GetCapabilitiesCommand
  | RelaySetCommand
  | RelayGetCommand
  | FanSetSpeedCommand
  | FanGetCommand
  | FanSetModeCommand
  | TemperatureReadCommand
  | LEDSetColorCommand
  | LEDSetBrightnessCommand
  | LEDSetAnimationCommand
  | ConfigGetCommand
  | ConfigSetCommand
  | ConfigBeginCommand
  | ConfigCommitCommand
  | GetAllStatusCommand
  | GetEventLogCommand;

// Response types
export interface CommandResponse {
  sequence: number;
  command: number;
  isAck: boolean;
  isNak: boolean;
  errorCode?: ErrorCode;
  payload?: Uint8Array;
}

export interface AckResponse extends CommandResponse {
  isAck: true;
  isNak: false;
}

export interface NakResponse extends CommandResponse {
  isAck: false;
  isNak: true;
  errorCode: ErrorCode;
}

export interface DataResponse extends CommandResponse {
  isAck: false;
  isNak: false;
  payload: Uint8Array;
}

// Capabilities response
export interface CapabilitiesResponse {
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

// Status response
export interface StatusResponse {
  relayStates: RelayState[];
  fanSpeeds: number[];
  temperatures: number[];
  drivePresent: boolean[];
  timestamp: number;
}

// Configuration response
export interface ConfigurationResponse {
  version: number;
  data: Uint8Array;
}

// Fan status response
export interface FanStatusResponse {
  fanId: number;
  speed: number;
  status: number;
}

// Temperature sensor response
export interface TemperatureSensorResponse {
  sensorId: number;
  temperature: number;
}

// Event log response
export interface EventLogResponse {
  entries: EventLogEntry[];
}

export interface EventLogEntry {
  index: number;
  timestamp: number;
  eventType: number;
  data: Uint8Array;
}
