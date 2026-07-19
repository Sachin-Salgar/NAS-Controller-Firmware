import {
  ErrorClassification,
  NASControllerError,
  TransportError,
  SerialDisconnectedError,
  SerialPortBusyError,
  SerialReadTimeoutError,
  ProtocolError,
  CRCValidationError,
  InvalidPacketFormatError,
  InvalidHeaderError,
  SequenceNumberMismatchError,
  FirmwareError,
  FirmwareTimeoutError,
  FirmwareNotReadyError,
  FirmwareInvalidCommandError,
  FirmwareOutOfRangeError,
  ValidationError,
  InvalidParameterError,
  OutOfRangeError,
  UserError,
  CommandWhileDisconnectedError,
  QueueFullError,
  InternalError,
  StateCorruptionError,
  UnexpectedResponseError,
  mapFirmwareErrorCode,
  isRetryableError,
  getErrorClassification,
} from "../errors";
import { ErrorCode } from "../protocol";

describe("Error Classification", () => {
  test("ErrorClassification enum has all types", () => {
    expect(ErrorClassification.TRANSPORT).toBe("transport");
    expect(ErrorClassification.PROTOCOL).toBe("protocol");
    expect(ErrorClassification.FIRMWARE).toBe("firmware");
    expect(ErrorClassification.VALIDATION).toBe("validation");
    expect(ErrorClassification.USER).toBe("user");
    expect(ErrorClassification.INTERNAL).toBe("internal");
  });
});

describe("Base NASControllerError", () => {
  test("creates error with all properties", () => {
    const error = new NASControllerError(
      "Test error",
      "TEST_ERROR",
      ErrorClassification.INTERNAL,
      false
    );
    expect(error.message).toBe("Test error");
    expect(error.code).toBe("TEST_ERROR");
    expect(error.classification).toBe(ErrorClassification.INTERNAL);
    expect(error.isRetryable).toBe(false);
  });

  test("error is instanceof Error", () => {
    const error = new NASControllerError(
      "Test",
      "TEST",
      ErrorClassification.INTERNAL
    );
    expect(error instanceof Error).toBe(true);
  });

  test("captures original error", () => {
    const original = new Error("Original");
    const error = new NASControllerError(
      "Wrapped",
      "WRAPPED",
      ErrorClassification.TRANSPORT,
      false,
      original
    );
    expect(error.originalError).toBe(original);
  });
});

describe("Transport Errors", () => {
  test("TransportError is retryable by default", () => {
    const error = new TransportError("Serial error");
    expect(error.isRetryable).toBe(true);
    expect(error.classification).toBe(ErrorClassification.TRANSPORT);
  });

  test("SerialDisconnectedError", () => {
    const error = new SerialDisconnectedError();
    expect(error.code).toBe("SERIAL_DISCONNECTED");
    expect(error.isRetryable).toBe(true);
  });

  test("SerialPortBusyError", () => {
    const error = new SerialPortBusyError("/dev/ttyUSB0");
    expect(error.code).toBe("SERIAL_PORT_BUSY");
    expect(error.message).toContain("/dev/ttyUSB0");
  });

  test("SerialReadTimeoutError", () => {
    const error = new SerialReadTimeoutError(1000);
    expect(error.code).toBe("SERIAL_READ_TIMEOUT");
    expect(error.message).toContain("1000");
  });
});

describe("Protocol Errors", () => {
  test("ProtocolError is not retryable by default", () => {
    const error = new ProtocolError("Protocol error");
    expect(error.isRetryable).toBe(false);
    expect(error.classification).toBe(ErrorClassification.PROTOCOL);
  });

  test("CRCValidationError shows expected vs actual", () => {
    const error = new CRCValidationError(0xabcd, 0x1234);
    expect(error.code).toBe("CRC_VALIDATION_FAILED");
    expect(error.message).toContain("0xabcd");
    expect(error.message).toContain("0x1234");
    expect(error.isRetryable).toBe(true);
  });

  test("InvalidPacketFormatError is not retryable", () => {
    const error = new InvalidPacketFormatError("Missing footer");
    expect(error.code).toBe("INVALID_PACKET_FORMAT");
    expect(error.isRetryable).toBe(false);
  });

  test("InvalidHeaderError", () => {
    const error = new InvalidHeaderError(0xabcd);
    expect(error.code).toBe("INVALID_HEADER");
    expect(error.message).toContain("0x55aa");
    expect(error.message).toContain("0xabcd");
    expect(error.isRetryable).toBe(true);
  });

  test("SequenceNumberMismatchError", () => {
    const error = new SequenceNumberMismatchError(5, 3);
    expect(error.code).toBe("SEQUENCE_MISMATCH");
    expect(error.message).toContain("5");
    expect(error.message).toContain("3");
    expect(error.isRetryable).toBe(true);
  });
});

describe("Firmware Errors", () => {
  test("FirmwareError stores error code", () => {
    const error = new FirmwareError("Device error", ErrorCode.OUT_OF_RANGE, false);
    expect(error.errorCode).toBe(ErrorCode.OUT_OF_RANGE);
    expect(error.classification).toBe(ErrorClassification.FIRMWARE);
  });

  test("FirmwareTimeoutError", () => {
    const error = new FirmwareTimeoutError();
    expect(error.errorCode).toBe(ErrorCode.TIMEOUT);
    expect(error.isRetryable).toBe(true);
  });

  test("FirmwareNotReadyError", () => {
    const error = new FirmwareNotReadyError();
    expect(error.errorCode).toBe(ErrorCode.DEVICE_NOT_READY);
    expect(error.isRetryable).toBe(true);
  });

  test("FirmwareInvalidCommandError", () => {
    const error = new FirmwareInvalidCommandError(0x10);
    expect(error.errorCode).toBe(ErrorCode.INVALID_COMMAND);
    expect(error.isRetryable).toBe(false);
    expect(error.message).toContain("0x10");
  });

  test("FirmwareOutOfRangeError", () => {
    const error = new FirmwareOutOfRangeError("speed", 150, 0, 100);
    expect(error.errorCode).toBe(ErrorCode.OUT_OF_RANGE);
    expect(error.message).toContain("speed");
    expect(error.message).toContain("150");
  });

  test("FirmwareConfigLockedError", () => {
    const error = new FirmwareConfigLockedError();
    expect(error.errorCode).toBe(ErrorCode.CONFIG_LOCKED);
    expect(error.isRetryable).toBe(false);
  });

  test("FirmwareConfigInvalidError", () => {
    const error = new FirmwareConfigInvalidError("invalid threshold");
    expect(error.errorCode).toBe(ErrorCode.CONFIG_INVALID);
    expect(error.message).toContain("invalid threshold");
  });
});

describe("Validation Errors", () => {
  test("ValidationError is not retryable", () => {
    const error = new ValidationError("Invalid input");
    expect(error.isRetryable).toBe(false);
    expect(error.classification).toBe(ErrorClassification.VALIDATION);
  });

  test("InvalidParameterError", () => {
    const error = new InvalidParameterError("speed", "must be between 0-100");
    expect(error.code).toBe("INVALID_PARAMETER");
    expect(error.message).toContain("speed");
  });

  test("OutOfRangeError", () => {
    const error = new OutOfRangeError("speed", 150, 0, 100);
    expect(error.code).toBe("OUT_OF_RANGE");
    expect(error.message).toContain("speed");
    expect(error.message).toContain("150");
  });
});

describe("User Errors", () => {
  test("UserError is not retryable", () => {
    const error = new UserError("User did something wrong");
    expect(error.isRetryable).toBe(false);
    expect(error.classification).toBe(ErrorClassification.USER);
  });

  test("CommandWhileDisconnectedError", () => {
    const error = new CommandWhileDisconnectedError();
    expect(error.code).toBe("COMMAND_WHILE_DISCONNECTED");
  });

  test("QueueFullError", () => {
    const error = new QueueFullError();
    expect(error.code).toBe("QUEUE_FULL");
  });
});

describe("Internal Errors", () => {
  test("InternalError is not retryable", () => {
    const error = new InternalError("Internal problem");
    expect(error.isRetryable).toBe(false);
    expect(error.classification).toBe(ErrorClassification.INTERNAL);
  });

  test("StateCorruptionError", () => {
    const error = new StateCorruptionError("relay state mismatch");
    expect(error.code).toBe("STATE_CORRUPTION");
    expect(error.message).toContain("relay state mismatch");
  });

  test("UnexpectedResponseError", () => {
    const error = new UnexpectedResponseError("ACK", "NAK");
    expect(error.code).toBe("UNEXPECTED_RESPONSE");
    expect(error.message).toContain("ACK");
    expect(error.message).toContain("NAK");
  });
});

describe("Error Code Mapping", () => {
  test("maps all error codes", () => {
    Object.values(ErrorCode).forEach((code) => {
      if (typeof code === "number") {
        const error = mapFirmwareErrorCode(code);
        expect(error).toBeInstanceOf(NASControllerError);
      }
    });
  });

  test("OK error code", () => {
    const error = mapFirmwareErrorCode(ErrorCode.OK);
    expect(error.code).toBe("OK");
  });

  test("INVALID_COMMAND error code", () => {
    const error = mapFirmwareErrorCode(ErrorCode.INVALID_COMMAND);
    expect(error).toBeInstanceOf(FirmwareInvalidCommandError);
  });

  test("TIMEOUT error code", () => {
    const error = mapFirmwareErrorCode(ErrorCode.TIMEOUT);
    expect(error).toBeInstanceOf(FirmwareTimeoutError);
  });

  test("OUT_OF_RANGE error code", () => {
    const error = mapFirmwareErrorCode(ErrorCode.OUT_OF_RANGE);
    expect(error).toBeInstanceOf(FirmwareOutOfRangeError);
  });

  test("CONFIG_LOCKED error code", () => {
    const error = mapFirmwareErrorCode(ErrorCode.CONFIG_LOCKED);
    expect(error).toBeInstanceOf(FirmwareConfigLockedError);
  });

  test("maps unknown error to FirmwareError", () => {
    const error = mapFirmwareErrorCode(ErrorCode.UNKNOWN_ERROR);
    expect(error).toBeInstanceOf(FirmwareError);
    expect(error.isRetryable).toBe(true);
  });

  test("includes context in error message", () => {
    const error = mapFirmwareErrorCode(ErrorCode.CONFIG_INVALID, "save");
    expect(error.message).toContain("save");
  });
});

describe("Error Helper Functions", () => {
  test("isRetryableError returns true for retryable errors", () => {
    const retryable = new TransportError("Timeout");
    expect(isRetryableError(retryable)).toBe(true);
  });

  test("isRetryableError returns false for non-retryable errors", () => {
    const nonRetryable = new FirmwareInvalidCommandError(0x10);
    expect(isRetryableError(nonRetryable)).toBe(false);
  });

  test("isRetryableError returns false for generic errors", () => {
    const error = new Error("Unknown error");
    expect(isRetryableError(error)).toBe(false);
  });

  test("getErrorClassification returns correct classification", () => {
    expect(getErrorClassification(new TransportError("error"))).toBe(
      ErrorClassification.TRANSPORT
    );
    expect(getErrorClassification(new ProtocolError("error"))).toBe(
      ErrorClassification.PROTOCOL
    );
    expect(getErrorClassification(new FirmwareError("error", ErrorCode.TIMEOUT))).toBe(
      ErrorClassification.FIRMWARE
    );
    expect(getErrorClassification(new ValidationError("error"))).toBe(
      ErrorClassification.VALIDATION
    );
    expect(getErrorClassification(new UserError("error"))).toBe(
      ErrorClassification.USER
    );
    expect(getErrorClassification(new InternalError("error"))).toBe(
      ErrorClassification.INTERNAL
    );
  });

  test("getErrorClassification returns INTERNAL for generic errors", () => {
    const error = new Error("Unknown");
    expect(getErrorClassification(error)).toBe(ErrorClassification.INTERNAL);
  });
});

describe("Error Inheritance Chain", () => {
  test("TransportError extends NASControllerError", () => {
    const error = new TransportError("error");
    expect(error instanceof NASControllerError).toBe(true);
    expect(error instanceof Error).toBe(true);
  });

  test("FirmwareError extends NASControllerError", () => {
    const error = new FirmwareError("error", ErrorCode.TIMEOUT);
    expect(error instanceof NASControllerError).toBe(true);
  });

  test("SerialDisconnectedError extends TransportError", () => {
    const error = new SerialDisconnectedError();
    expect(error instanceof TransportError).toBe(true);
    expect(error instanceof NASControllerError).toBe(true);
  });

  test("InvalidParameterError extends ValidationError", () => {
    const error = new InvalidParameterError("param", "reason");
    expect(error instanceof ValidationError).toBe(true);
    expect(error instanceof NASControllerError).toBe(true);
  });
});
