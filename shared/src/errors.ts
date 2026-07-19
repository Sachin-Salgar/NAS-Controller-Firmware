import { ErrorCode } from "./protocol";

// Error classification types
export enum ErrorClassification {
  TRANSPORT = "transport",
  PROTOCOL = "protocol",
  FIRMWARE = "firmware",
  VALIDATION = "validation",
  USER = "user",
  INTERNAL = "internal",
}

// Base error class
export class NASControllerError extends Error {
  public readonly code: string;
  public readonly classification: ErrorClassification;
  public readonly isRetryable: boolean;
  public readonly originalError?: Error;

  constructor(
    message: string,
    code: string,
    classification: ErrorClassification,
    isRetryable: boolean = false,
    originalError?: Error
  ) {
    super(message);
    this.name = "NASControllerError";
    this.code = code;
    this.classification = classification;
    this.isRetryable = isRetryable;
    this.originalError = originalError;

    Object.setPrototypeOf(this, NASControllerError.prototype);
  }
}

// Transport layer errors
export class TransportError extends NASControllerError {
  constructor(message: string, isRetryable: boolean = true, originalError?: Error) {
    super(
      message,
      "TRANSPORT_ERROR",
      ErrorClassification.TRANSPORT,
      isRetryable,
      originalError
    );
    Object.setPrototypeOf(this, TransportError.prototype);
  }
}

export class SerialDisconnectedError extends TransportError {
  constructor(originalError?: Error) {
    super("Serial device disconnected", true, originalError);
    this.code = "SERIAL_DISCONNECTED";
  }
}

export class SerialPortBusyError extends TransportError {
  constructor(port: string, originalError?: Error) {
    super(`Serial port ${port} is busy`, true, originalError);
    this.code = "SERIAL_PORT_BUSY";
  }
}

export class SerialReadTimeoutError extends TransportError {
  constructor(timeoutMs: number, originalError?: Error) {
    super(`Serial read timeout after ${timeoutMs}ms`, true, originalError);
    this.code = "SERIAL_READ_TIMEOUT";
  }
}

export class ConnectionLostError extends TransportError {
  constructor(originalError?: Error) {
    super("Connection to device lost", true, originalError);
    this.code = "CONNECTION_LOST";
  }
}

// Protocol layer errors
export class ProtocolError extends NASControllerError {
  constructor(message: string, isRetryable: boolean = false, originalError?: Error) {
    super(
      message,
      "PROTOCOL_ERROR",
      ErrorClassification.PROTOCOL,
      isRetryable,
      originalError
    );
    Object.setPrototypeOf(this, ProtocolError.prototype);
  }
}

export class CRCValidationError extends ProtocolError {
  constructor(expected: number, actual: number, originalError?: Error) {
    super(
      `CRC validation failed: expected 0x${expected.toString(16)}, got 0x${actual.toString(
        16
      )}`,
      true,
      originalError
    );
    this.code = "CRC_VALIDATION_FAILED";
  }
}

export class InvalidPacketFormatError extends ProtocolError {
  constructor(reason: string, originalError?: Error) {
    super(`Invalid packet format: ${reason}`, false, originalError);
    this.code = "INVALID_PACKET_FORMAT";
  }
}

export class InvalidHeaderError extends ProtocolError {
  constructor(received: number, originalError?: Error) {
    super(
      `Invalid packet header: expected 0x55AA, got 0x${received.toString(16)}`,
      true,
      originalError
    );
    this.code = "INVALID_HEADER";
  }
}

export class SequenceNumberMismatchError extends ProtocolError {
  constructor(expected: number, received: number, originalError?: Error) {
    super(
      `Sequence number mismatch: expected ${expected}, got ${received}`,
      true,
      originalError
    );
    this.code = "SEQUENCE_MISMATCH";
  }
}

// Firmware errors
export class FirmwareError extends NASControllerError {
  public readonly errorCode: ErrorCode;

  constructor(
    message: string,
    errorCode: ErrorCode,
    isRetryable: boolean = false,
    originalError?: Error
  ) {
    super(
      message,
      `FIRMWARE_ERROR_${errorCode.toString().padStart(2, "0")}`,
      ErrorClassification.FIRMWARE,
      isRetryable,
      originalError
    );
    this.errorCode = errorCode;
    Object.setPrototypeOf(this, FirmwareError.prototype);
  }
}

export class FirmwareTimeoutError extends FirmwareError {
  constructor(originalError?: Error) {
    super(
      "Device did not respond within timeout period",
      ErrorCode.TIMEOUT,
      true,
      originalError
    );
  }
}

export class FirmwareNotReadyError extends FirmwareError {
  constructor(originalError?: Error) {
    super("Device is not ready", ErrorCode.DEVICE_NOT_READY, true, originalError);
  }
}

export class FirmwareInvalidCommandError extends FirmwareError {
  constructor(command: number, originalError?: Error) {
    super(
      `Device rejected command: 0x${command.toString(16)}`,
      ErrorCode.INVALID_COMMAND,
      false,
      originalError
    );
  }
}

export class FirmwareInvalidPayloadError extends FirmwareError {
  constructor(reason: string, originalError?: Error) {
    super(
      `Device rejected payload: ${reason}`,
      ErrorCode.INVALID_PAYLOAD,
      false,
      originalError
    );
  }
}

export class FirmwareOutOfRangeError extends FirmwareError {
  constructor(parameter: string, value: number, min: number, max: number, originalError?: Error) {
    super(
      `Parameter ${parameter} out of range: ${value} (valid: ${min}-${max})`,
      ErrorCode.OUT_OF_RANGE,
      false,
      originalError
    );
  }
}

export class FirmwareConfigLockedError extends FirmwareError {
  constructor(originalError?: Error) {
    super(
      "Configuration is locked (transaction in progress)",
      ErrorCode.CONFIG_LOCKED,
      false,
      originalError
    );
  }
}

export class FirmwareConfigInvalidError extends FirmwareError {
  constructor(reason: string, originalError?: Error) {
    super(
      `Configuration validation failed: ${reason}`,
      ErrorCode.CONFIG_INVALID,
      false,
      originalError
    );
  }
}

// Validation errors
export class ValidationError extends NASControllerError {
  constructor(message: string, originalError?: Error) {
    super(
      message,
      "VALIDATION_ERROR",
      ErrorClassification.VALIDATION,
      false,
      originalError
    );
    Object.setPrototypeOf(this, ValidationError.prototype);
  }
}

export class InvalidParameterError extends ValidationError {
  constructor(paramName: string, reason: string, originalError?: Error) {
    super(`Invalid parameter ${paramName}: ${reason}`, originalError);
    this.code = "INVALID_PARAMETER";
  }
}

export class OutOfRangeError extends ValidationError {
  constructor(
    paramName: string,
    value: number,
    min: number,
    max: number,
    originalError?: Error
  ) {
    super(
      `Parameter ${paramName} out of range: ${value} (valid: ${min}-${max})`,
      originalError
    );
    this.code = "OUT_OF_RANGE";
  }
}

// User errors (user-caused, not system errors)
export class UserError extends NASControllerError {
  constructor(message: string, originalError?: Error) {
    super(message, "USER_ERROR", ErrorClassification.USER, false, originalError);
    Object.setPrototypeOf(this, UserError.prototype);
  }
}

export class CommandWhileDisconnectedError extends UserError {
  constructor(originalError?: Error) {
    super("Cannot send command while device is disconnected", originalError);
    this.code = "COMMAND_WHILE_DISCONNECTED";
  }
}

export class QueueFullError extends UserError {
  constructor(originalError?: Error) {
    super("Command queue is full, please wait", originalError);
    this.code = "QUEUE_FULL";
  }
}

// Internal errors
export class InternalError extends NASControllerError {
  constructor(message: string, originalError?: Error) {
    super(message, "INTERNAL_ERROR", ErrorClassification.INTERNAL, false, originalError);
    Object.setPrototypeOf(this, InternalError.prototype);
  }
}

export class StateCorruptionError extends InternalError {
  constructor(message: string, originalError?: Error) {
    super(`State corruption detected: ${message}`, originalError);
    this.code = "STATE_CORRUPTION";
  }
}

export class UnexpectedResponseError extends InternalError {
  constructor(expected: string, received: string, originalError?: Error) {
    super(`Unexpected response: expected ${expected}, got ${received}`, originalError);
    this.code = "UNEXPECTED_RESPONSE";
  }
}

// Error mapping from firmware error codes
export function mapFirmwareErrorCode(
  errorCode: ErrorCode,
  context?: string
): NASControllerError {
  const baseMessage = context ? `${context}: ` : "";

  switch (errorCode) {
    case ErrorCode.OK:
      return new NASControllerError("No error", "OK", ErrorClassification.INTERNAL);

    case ErrorCode.INVALID_COMMAND:
      return new FirmwareInvalidCommandError(0);

    case ErrorCode.INVALID_PAYLOAD:
      return new FirmwareInvalidPayloadError("payload structure invalid");

    case ErrorCode.INVALID_SEQUENCE:
      return new ProtocolError(`${baseMessage}Sequence number out of order`, true);

    case ErrorCode.CRC_FAILED:
      return new CRCValidationError(0, 0);

    case ErrorCode.DEVICE_NOT_READY:
      return new FirmwareNotReadyError();

    case ErrorCode.TIMEOUT:
      return new FirmwareTimeoutError();

    case ErrorCode.OUT_OF_RANGE:
      return new FirmwareOutOfRangeError("parameter", 0, 0, 0);

    case ErrorCode.CONFIG_LOCKED:
      return new FirmwareConfigLockedError();

    case ErrorCode.CONFIG_INVALID:
      return new FirmwareConfigInvalidError(baseMessage);

    case ErrorCode.UNKNOWN_ERROR:
    default:
      return new FirmwareError(
        `${baseMessage}Unknown firmware error`,
        errorCode,
        true
      );
  }
}

// Helper to determine if error is retryable
export function isRetryableError(error: Error): boolean {
  if (error instanceof NASControllerError) {
    return error.isRetryable;
  }
  return false;
}

// Helper to get error classification
export function getErrorClassification(error: Error): ErrorClassification {
  if (error instanceof NASControllerError) {
    return error.classification;
  }
  return ErrorClassification.INTERNAL;
}
