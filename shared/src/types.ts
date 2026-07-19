// Common numeric types
export type Uint8 = number;
export type Uint16 = number;
export type Uint32 = number;
export type Int16 = number;

// Relay types
export interface RelayInfo {
  id: number;
  state: boolean;
  label?: string;
}

export interface RelayStatus {
  relays: RelayInfo[];
}

// Fan types
export interface FanInfo {
  id: number;
  speed: number;
  mode: "manual" | "auto" | "off";
  status: "ok" | "stalled" | "overtemp";
  label?: string;
}

export interface FanStatus {
  fans: FanInfo[];
}

// Temperature sensor types
export interface TemperatureSensorInfo {
  id: number;
  temperature: number;
  unit: "celsius" | "fahrenheit";
  label?: string;
}

export interface TemperatureStatus {
  sensors: TemperatureSensorInfo[];
}

// LED types
export interface LEDInfo {
  id: number;
  red: number;
  green: number;
  blue: number;
  brightness: number;
  animation: number;
  label?: string;
}

export interface LEDStatus {
  leds: LEDInfo[];
}

// Drive types
export interface DriveInfo {
  id: number;
  present: boolean;
  label?: string;
  capacity?: number;
}

export interface DriveStatus {
  drives: DriveInfo[];
}

// System health types
export enum HealthLevel {
  HEALTHY = "healthy",
  WARNING = "warning",
  CRITICAL = "critical",
}

export interface SystemHealth {
  level: HealthLevel;
  message: string;
  timestamp: number;
}

// Capabilities types
export interface FirmwareCapabilities {
  protocolVersion: number;
  configVersion: number;
  firmwareVersion: string;
  hardwareVersion?: string;
  relayCount: number;
  fanCount: number;
  ledCount: number;
  temperatureSensorCount: number;
  driveCount: number;
  supportedCommands: number[];
  uptime?: number;
}

// Configuration types
export interface ConfigurationThresholds {
  fanAutoMinSpeed: number;
  fanAutoMaxSpeed: number;
  tempWarningThreshold: number;
  tempCriticalThreshold: number;
}

export interface ConfigurationSettings {
  thresholds: ConfigurationThresholds;
  autoStartEnabled: boolean;
  debugMode: boolean;
  customData?: Record<string, unknown>;
}

// Statistics types
export interface SystemStatistics {
  uptime: number;
  commandsSent: number;
  commandsFailed: number;
  commandsRetried: number;
  errorCount: number;
  lastErrorTime?: number;
  lastErrorMessage?: string;
  averageCommandLatency: number;
}

// Event types
export enum EventType {
  DEVICE_CONNECTED = "device_connected",
  DEVICE_DISCONNECTED = "device_disconnected",
  COMMAND_SENT = "command_sent",
  COMMAND_COMPLETED = "command_completed",
  COMMAND_FAILED = "command_failed",
  STATE_CHANGED = "state_changed",
  ERROR_OCCURRED = "error_occurred",
  CONFIGURATION_CHANGED = "configuration_changed",
  RELAY_CHANGED = "relay_changed",
  FAN_CHANGED = "fan_changed",
  LED_CHANGED = "led_changed",
  TEMPERATURE_CHANGED = "temperature_changed",
}

export interface Event {
  id: string;
  type: EventType;
  timestamp: number;
  source: string;
  data: Record<string, unknown>;
  severity: "info" | "warning" | "error";
}

// Complete state snapshots
export interface HardwareState {
  relays: RelayStatus;
  fans: FanStatus;
  temperatures: TemperatureStatus;
  leds: LEDStatus;
  drives: DriveStatus;
  health: SystemHealth;
}

export interface FirmwareState {
  connected: boolean;
  capabilities?: FirmwareCapabilities;
  hardware: HardwareState;
  lastUpdate: number;
}

export interface ApplicationState {
  firmware: FirmwareState;
  configuration: ConfigurationSettings;
  statistics: SystemStatistics;
  lastStatusUpdate: number;
  lastConfigUpdate: number;
}

// Command execution result
export interface CommandExecutionResult {
  commandId: string;
  commandType: string;
  sequence: number;
  success: boolean;
  startTime: number;
  endTime: number;
  duration: number;
  attempts: number;
  error?: {
    code: string;
    message: string;
    classification: string;
    retryable: boolean;
  };
  response?: {
    isAck: boolean;
    isNak: boolean;
    errorCode?: number;
    payload?: Uint8Array;
  };
}

// Subscription callback type
export type StateSubscriber = (state: ApplicationState) => void;
export type EventSubscriber = (event: Event) => void;

// Unsubscribe function type
export type Unsubscribe = () => void;

// Logger interface
export interface Logger {
  debug(message: string, context?: Record<string, unknown>): void;
  info(message: string, context?: Record<string, unknown>): void;
  warn(message: string, context?: Record<string, unknown>): void;
  error(message: string, context?: Record<string, unknown>): void;
}

// Transport interface (abstraction for different transport mechanisms)
export interface ITransport {
  connect(): Promise<void>;
  disconnect(): Promise<void>;
  send(data: Uint8Array): Promise<void>;
  receive(): Promise<Uint8Array>;
  isConnected(): boolean;

  // Event handlers
  onConnect(handler: () => void): void;
  onDisconnect(handler: () => void): void;
  onData(handler: (data: Uint8Array) => void): void;
  onError(handler: (error: Error) => void): void;
}

// Queue item interface
export interface QueuedCommand {
  id: string;
  command: Record<string, unknown>;
  sequence: number;
  timestamp: number;
  state: "queued" | "sending" | "waiting_ack" | "completed" | "failed" | "timeout";
  attempts: number;
  error?: Error;
}

// Configuration storage interface
export interface IConfigurationStore {
  load(): Promise<ConfigurationSettings>;
  save(config: ConfigurationSettings): Promise<void>;
}
