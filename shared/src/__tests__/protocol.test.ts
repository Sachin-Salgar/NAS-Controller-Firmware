import {
  PACKET_HEADER,
  PACKET_FOOTER,
  MAX_PAYLOAD_SIZE,
  PROTOCOL_VERSION,
  CRC16_INITIAL_VALUE,
  CRC16_POLYNOMIAL,
  ACK_MASK,
  NAK_MASK,
  RESPONSE_MASK,
  COMMAND_TIMEOUT_MS,
  MAX_RETRY_ATTEMPTS,
  ErrorCode,
  RelayState,
  FanMode,
  LEDAnimation,
  CommandState,
  CMD_SYSTEM_PING,
  CMD_RELAY_SET,
  CMD_RELAY_GET,
  CMD_FAN_SET_SPEED,
  CMD_FAN_GET,
  CMD_FAN_SET_MODE,
  CMD_TEMP_READ,
  CMD_LED_SET_COLOR,
  CMD_LED_SET_BRIGHTNESS,
  CMD_LED_SET_ANIMATION,
  CMD_CONFIG_GET,
  CMD_CONFIG_SET,
  CMD_CONFIG_BEGIN,
  CMD_CONFIG_COMMIT,
  CMD_GET_ALL_STATUS,
  CMD_GET_EVENT_LOG,
  PingCommand,
  RelaySetCommand,
  FanSetSpeedCommand,
  LEDSetColorCommand,
  CommandResponse,
  AckResponse,
  NakResponse,
  CapabilitiesResponse,
  StatusResponse,
} from "../protocol";

describe("Protocol Constants", () => {
  test("packet header matches specification", () => {
    expect(PACKET_HEADER).toBe(0x55aa);
  });

  test("packet footer matches specification", () => {
    expect(PACKET_FOOTER).toBe(0xaa);
  });

  test("max payload size is 256 bytes", () => {
    expect(MAX_PAYLOAD_SIZE).toBe(256);
  });

  test("protocol version is 1", () => {
    expect(PROTOCOL_VERSION).toBe(1);
  });

  test("CRC16 initial value is correct", () => {
    expect(CRC16_INITIAL_VALUE).toBe(0xffff);
  });

  test("CRC16 polynomial is CCITT", () => {
    expect(CRC16_POLYNOMIAL).toBe(0x1021);
  });

  test("ACK mask is 0x80", () => {
    expect(ACK_MASK).toBe(0x80);
  });

  test("NAK mask is 0x40", () => {
    expect(NAK_MASK).toBe(0x40);
  });

  test("response mask is 0x90", () => {
    expect(RESPONSE_MASK).toBe(0x90);
  });

  test("command timeout is 1000ms", () => {
    expect(COMMAND_TIMEOUT_MS).toBe(1000);
  });

  test("max retry attempts is 3", () => {
    expect(MAX_RETRY_ATTEMPTS).toBe(3);
  });
});

describe("Command Code Constants", () => {
  test("system command codes are defined", () => {
    expect(CMD_SYSTEM_PING).toBe(0x01);
  });

  test("relay command codes match specification", () => {
    expect(CMD_RELAY_SET).toBe(0x10);
    expect(CMD_RELAY_GET).toBe(0x11);
  });

  test("fan command codes match specification", () => {
    expect(CMD_FAN_SET_SPEED).toBe(0x30);
    expect(CMD_FAN_GET).toBe(0x31);
    expect(CMD_FAN_SET_MODE).toBe(0x32);
  });

  test("temperature command codes match specification", () => {
    expect(CMD_TEMP_READ).toBe(0x40);
  });

  test("LED command codes match specification", () => {
    expect(CMD_LED_SET_COLOR).toBe(0x50);
    expect(CMD_LED_SET_BRIGHTNESS).toBe(0x51);
    expect(CMD_LED_SET_ANIMATION).toBe(0x52);
  });

  test("configuration command codes match specification", () => {
    expect(CMD_CONFIG_GET).toBe(0x60);
    expect(CMD_CONFIG_SET).toBe(0x61);
    expect(CMD_CONFIG_BEGIN).toBe(0x62);
    expect(CMD_CONFIG_COMMIT).toBe(0x63);
  });

  test("status command codes match specification", () => {
    expect(CMD_GET_ALL_STATUS).toBe(0x20);
  });

  test("event log command codes match specification", () => {
    expect(CMD_GET_EVENT_LOG).toBe(0x70);
  });
});

describe("Enumerations", () => {
  test("ErrorCode enum values are correct", () => {
    expect(ErrorCode.OK).toBe(0x00);
    expect(ErrorCode.INVALID_COMMAND).toBe(0x01);
    expect(ErrorCode.INVALID_PAYLOAD).toBe(0x02);
    expect(ErrorCode.INVALID_SEQUENCE).toBe(0x03);
    expect(ErrorCode.CRC_FAILED).toBe(0x04);
    expect(ErrorCode.DEVICE_NOT_READY).toBe(0x05);
    expect(ErrorCode.TIMEOUT).toBe(0x06);
    expect(ErrorCode.OUT_OF_RANGE).toBe(0x07);
    expect(ErrorCode.CONFIG_LOCKED).toBe(0x08);
    expect(ErrorCode.CONFIG_INVALID).toBe(0x09);
    expect(ErrorCode.UNKNOWN_ERROR).toBe(0x0a);
  });

  test("RelayState enum values are correct", () => {
    expect(RelayState.OFF).toBe(0x00);
    expect(RelayState.ON).toBe(0x01);
  });

  test("FanMode enum values are correct", () => {
    expect(FanMode.MANUAL).toBe(0x00);
    expect(FanMode.AUTO).toBe(0x01);
    expect(FanMode.OFF).toBe(0x02);
  });

  test("LEDAnimation enum values are correct", () => {
    expect(LEDAnimation.BOOT).toBe(0x00);
    expect(LEDAnimation.IDLE).toBe(0x01);
    expect(LEDAnimation.ERROR).toBe(0x02);
    expect(LEDAnimation.SHUTDOWN).toBe(0x03);
    expect(LEDAnimation.CUSTOM).toBe(0x04);
  });

  test("CommandState enum values are correct", () => {
    expect(CommandState.QUEUED).toBe("queued");
    expect(CommandState.SENDING).toBe("sending");
    expect(CommandState.WAITING_ACK).toBe("waiting_ack");
    expect(CommandState.COMPLETED).toBe("completed");
    expect(CommandState.FAILED).toBe("failed");
    expect(CommandState.TIMEOUT).toBe("timeout");
  });
});

describe("Command Type Definitions", () => {
  test("PingCommand type structure", () => {
    const ping: PingCommand = {
      sequence: 1,
      command: CMD_SYSTEM_PING,
    };
    expect(ping.sequence).toBe(1);
    expect(ping.command).toBe(CMD_SYSTEM_PING);
  });

  test("RelaySetCommand type structure", () => {
    const relay: RelaySetCommand = {
      sequence: 2,
      command: CMD_RELAY_SET,
      relayId: 0,
      state: RelayState.ON,
    };
    expect(relay.relayId).toBe(0);
    expect(relay.state).toBe(RelayState.ON);
  });

  test("FanSetSpeedCommand type structure", () => {
    const fan: FanSetSpeedCommand = {
      sequence: 3,
      command: CMD_FAN_SET_SPEED,
      fanId: 0,
      speed: 50,
    };
    expect(fan.fanId).toBe(0);
    expect(fan.speed).toBe(50);
  });

  test("LEDSetColorCommand type structure", () => {
    const led: LEDSetColorCommand = {
      sequence: 4,
      command: CMD_LED_SET_COLOR,
      red: 255,
      green: 128,
      blue: 0,
    };
    expect(led.red).toBe(255);
    expect(led.green).toBe(128);
    expect(led.blue).toBe(0);
  });
});

describe("Response Type Definitions", () => {
  test("CommandResponse base type", () => {
    const response: CommandResponse = {
      sequence: 1,
      command: CMD_SYSTEM_PING,
      isAck: true,
      isNak: false,
    };
    expect(response.isAck).toBe(true);
    expect(response.isNak).toBe(false);
  });

  test("AckResponse type", () => {
    const ack: AckResponse = {
      sequence: 1,
      command: CMD_RELAY_SET,
      isAck: true,
      isNak: false,
    };
    expect(ack.isAck).toBe(true);
  });

  test("NakResponse type with error code", () => {
    const nak: NakResponse = {
      sequence: 1,
      command: CMD_RELAY_SET,
      isAck: false,
      isNak: true,
      errorCode: ErrorCode.OUT_OF_RANGE,
    };
    expect(nak.isNak).toBe(true);
    expect(nak.errorCode).toBe(ErrorCode.OUT_OF_RANGE);
  });

  test("CapabilitiesResponse type", () => {
    const caps: CapabilitiesResponse = {
      protocolVersion: 1,
      configVersion: 1,
      firmwareVersion: 1,
      relayCount: 4,
      fanCount: 2,
      ledCount: 1,
      temperatureSensorCount: 3,
      driveCount: 4,
      supportedCommands: new Uint8Array([0x01, 0x10, 0x11]),
    };
    expect(caps.relayCount).toBe(4);
    expect(caps.fanCount).toBe(2);
  });

  test("StatusResponse type", () => {
    const status: StatusResponse = {
      relayStates: [RelayState.ON, RelayState.OFF, RelayState.ON, RelayState.OFF],
      fanSpeeds: [50, 75],
      temperatures: [22, 25, 28],
      drivePresent: [true, true, false, true],
      timestamp: Date.now(),
    };
    expect(status.relayStates.length).toBe(4);
    expect(status.fanSpeeds.length).toBe(2);
  });
});

describe("Type Safety", () => {
  test("all command codes are unique", () => {
    const codes = [
      CMD_SYSTEM_PING,
      CMD_RELAY_SET,
      CMD_RELAY_GET,
      CMD_FAN_SET_SPEED,
      CMD_FAN_GET,
      CMD_FAN_SET_MODE,
      CMD_TEMP_READ,
      CMD_LED_SET_COLOR,
      CMD_LED_SET_BRIGHTNESS,
      CMD_LED_SET_ANIMATION,
      CMD_CONFIG_GET,
      CMD_CONFIG_SET,
      CMD_CONFIG_BEGIN,
      CMD_CONFIG_COMMIT,
      CMD_GET_ALL_STATUS,
      CMD_GET_EVENT_LOG,
    ];
    const unique = new Set(codes);
    expect(unique.size).toBe(codes.length);
  });

  test("command codes do not overlap with ACK/NAK masks", () => {
    const regularCommands = [
      CMD_SYSTEM_PING,
      CMD_RELAY_SET,
      CMD_RELAY_GET,
      CMD_FAN_SET_SPEED,
    ];
    regularCommands.forEach((cmd) => {
      expect(cmd & ACK_MASK).toBe(0);
      expect(cmd & NAK_MASK).toBe(0);
    });
  });

  test("error codes are in valid range", () => {
    Object.values(ErrorCode).forEach((code) => {
      if (typeof code === "number") {
        expect(code).toBeGreaterThanOrEqual(0x00);
        expect(code).toBeLessThanOrEqual(0x0a);
      }
    });
  });
});
