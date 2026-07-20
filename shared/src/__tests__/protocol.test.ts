import {
  ProtocolInfo,
  ResponseBit,
  CommandCode,
  ErrorCode,
  RelayState,
  FanMode,
  LEDAnimation,
  EventType,
  PacketHeader,
  PacketMetadata,
  PacketFooter,
  Packet,
  ParsedPacket,
  CapabilityInfo,
  RelayConfig,
  FanConfig,
  TemperatureSensorConfig,
  DriveConfig,
  LEDConfig,
  RelayStatus,
  FanStatus,
  TemperatureStatus,
  DriveStatus,
  LEDStatus,
  SystemStatus,
  ConfigurationData,
  EventLogEntry,
  SystemPingRequest,
  RelaySetRequest,
  FanSetSpeedRequest,
  LEDSetColorRequest,
  GetAllStatusRequest,
  AckResponse,
  NakResponse,
  DataResponse,
  Result,
  Ok,
  Err,
} from "../protocol";

describe("ProtocolInfo", () => {
  test("protocol version is 1.0.0", () => {
    expect(ProtocolInfo.Version).toBe("1.0.0");
  });

  test("packet header is 0x55AA", () => {
    expect(ProtocolInfo.Header).toBe(0x55aa);
  });

  test("packet footer is 0xAA", () => {
    expect(ProtocolInfo.Footer).toBe(0xaa);
  });

  test("CRC algorithm is CRC16-CCITT", () => {
    expect(ProtocolInfo.CRC).toBe("CRC16-CCITT");
  });

  test("CRC initial value is 0xFFFF", () => {
    expect(ProtocolInfo.CRCInitialValue).toBe(0xffff);
  });

  test("CRC polynomial is 0x1021", () => {
    expect(ProtocolInfo.CRCPolynomial).toBe(0x1021);
  });

  test("max packet size is 256 bytes", () => {
    expect(ProtocolInfo.MaxPacketSize).toBe(256);
  });

  test("command timeout is 1000ms", () => {
    expect(ProtocolInfo.CommandTimeout).toBe(1000);
  });

  test("max retry attempts is 3", () => {
    expect(ProtocolInfo.MaxRetryAttempts).toBe(3);
  });
});

describe("ResponseBits", () => {
  test("ACK bit is 0x80", () => {
    expect(ResponseBit.ACK).toBe(0x80);
  });

  test("NAK bit is 0x40", () => {
    expect(ResponseBit.NAK).toBe(0x40);
  });

  test("Response bit is 0x90", () => {
    expect(ResponseBit.Response).toBe(0x90);
  });
});

describe("CommandCodes", () => {
  test("system command codes are defined", () => {
    expect(CommandCode.SYSTEM_PING).toBe(0x01);
    expect(CommandCode.SYSTEM_RESET).toBe(0x02);
    expect(CommandCode.GET_CAPABILITIES).toBe(0x03);
  });

  test("relay command codes match specification", () => {
    expect(CommandCode.RELAY_SET).toBe(0x10);
    expect(CommandCode.RELAY_GET).toBe(0x11);
  });

  test("status command codes are defined", () => {
    expect(CommandCode.GET_ALL_STATUS).toBe(0x20);
  });

  test("fan command codes match specification", () => {
    expect(CommandCode.FAN_SET_SPEED).toBe(0x30);
    expect(CommandCode.FAN_GET).toBe(0x31);
    expect(CommandCode.FAN_SET_MODE).toBe(0x32);
  });

  test("temperature command codes are defined", () => {
    expect(CommandCode.TEMP_READ).toBe(0x40);
  });

  test("LED command codes match specification", () => {
    expect(CommandCode.LED_SET_COLOR).toBe(0x50);
    expect(CommandCode.LED_SET_BRIGHTNESS).toBe(0x51);
    expect(CommandCode.LED_SET_ANIMATION).toBe(0x52);
  });

  test("configuration command codes match specification", () => {
    expect(CommandCode.CONFIG_GET).toBe(0x60);
    expect(CommandCode.CONFIG_SET).toBe(0x61);
    expect(CommandCode.CONFIG_BEGIN).toBe(0x62);
    expect(CommandCode.CONFIG_COMMIT).toBe(0x63);
  });

  test("event log command codes are defined", () => {
    expect(CommandCode.GET_EVENT_LOG).toBe(0x70);
  });

  test("all command codes are unique", () => {
    const codes = Object.values(CommandCode);
    const unique = new Set(codes);
    expect(unique.size).toBe(codes.length);
  });
});

describe("ErrorCode Enumeration", () => {
  test("error codes match specification", () => {
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
});

describe("Enumeration Values", () => {
  test("RelayState enum values", () => {
    expect(RelayState.OFF).toBe(0x00);
    expect(RelayState.ON).toBe(0x01);
  });

  test("FanMode enum values", () => {
    expect(FanMode.MANUAL).toBe(0x00);
    expect(FanMode.AUTO).toBe(0x01);
    expect(FanMode.OFF).toBe(0x02);
  });

  test("LEDAnimation enum values", () => {
    expect(LEDAnimation.BOOT).toBe(0x00);
    expect(LEDAnimation.IDLE).toBe(0x01);
    expect(LEDAnimation.ERROR).toBe(0x02);
    expect(LEDAnimation.SHUTDOWN).toBe(0x03);
    expect(LEDAnimation.CUSTOM).toBe(0x04);
  });
});

describe("EventType Enumeration", () => {
  test("device events are defined", () => {
    expect(EventType.DEVICE_CONNECTED).toBe("device_connected");
    expect(EventType.DEVICE_DISCONNECTED).toBe("device_disconnected");
  });

  test("hardware state change events are defined", () => {
    expect(EventType.RELAY_STATE_CHANGED).toBe("relay_state_changed");
    expect(EventType.FAN_SPEED_CHANGED).toBe("fan_speed_changed");
    expect(EventType.TEMPERATURE_CHANGED).toBe("temperature_changed");
  });

  test("command lifecycle events are defined", () => {
    expect(EventType.COMMAND_SENT).toBe("command_sent");
    expect(EventType.COMMAND_COMPLETED).toBe("command_completed");
    expect(EventType.COMMAND_FAILED).toBe("command_failed");
  });
});

describe("Packet Structure Types", () => {
  test("PacketHeader interface", () => {
    const header: PacketHeader = { magic: ProtocolInfo.Header };
    expect(header.magic).toBe(0x55aa);
  });

  test("PacketMetadata interface", () => {
    const metadata: PacketMetadata = {
      sequence: 1,
      command: CommandCode.SYSTEM_PING,
      length: 0,
    };
    expect(metadata.sequence).toBe(1);
    expect(metadata.command).toBe(0x01);
  });

  test("PacketFooter interface", () => {
    const footer: PacketFooter = {
      crc16: 0xabcd,
      terminator: ProtocolInfo.Footer,
    };
    expect(footer.crc16).toBe(0xabcd);
    expect(footer.terminator).toBe(0xaa);
  });

  test("Packet interface", () => {
    const packet: Packet = {
      header: { magic: ProtocolInfo.Header },
      metadata: {
        sequence: 1,
        command: CommandCode.SYSTEM_PING,
        length: 0,
      },
      payload: new Uint8Array(0),
      footer: {
        crc16: 0x0000,
        terminator: ProtocolInfo.Footer,
      },
    };
    expect(packet.metadata.sequence).toBe(1);
  });
});

describe("Request Types", () => {
  test("SystemPingRequest", () => {
    const req: SystemPingRequest = {
      sequence: 1,
      command: CommandCode.SYSTEM_PING,
    };
    expect(req.command).toBe(CommandCode.SYSTEM_PING);
  });

  test("RelaySetRequest", () => {
    const req: RelaySetRequest = {
      sequence: 2,
      command: CommandCode.RELAY_SET,
      relayId: 0,
      state: RelayState.ON,
    };
    expect(req.relayId).toBe(0);
    expect(req.state).toBe(RelayState.ON);
  });

  test("FanSetSpeedRequest", () => {
    const req: FanSetSpeedRequest = {
      sequence: 3,
      command: CommandCode.FAN_SET_SPEED,
      fanId: 0,
      speed: 50,
    };
    expect(req.speed).toBe(50);
  });

  test("LEDSetColorRequest", () => {
    const req: LEDSetColorRequest = {
      sequence: 4,
      command: CommandCode.LED_SET_COLOR,
      red: 255,
      green: 128,
      blue: 0,
    };
    expect(req.red).toBe(255);
  });
});

describe("Response Types", () => {
  test("AckResponse", () => {
    const response: AckResponse = {
      sequence: 1,
      command: CommandCode.RELAY_SET,
      isAck: true,
      isNak: false,
    };
    expect(response.isAck).toBe(true);
    expect(response.isNak).toBe(false);
  });

  test("NakResponse with error code", () => {
    const response: NakResponse = {
      sequence: 1,
      command: CommandCode.RELAY_SET,
      isAck: false,
      isNak: true,
      errorCode: ErrorCode.OUT_OF_RANGE,
    };
    expect(response.errorCode).toBe(ErrorCode.OUT_OF_RANGE);
  });

  test("DataResponse with payload", () => {
    const response: DataResponse = {
      sequence: 1,
      command: CommandCode.GET_ALL_STATUS,
      isAck: false,
      isNak: false,
      payload: new Uint8Array([0x01, 0x02, 0x03]),
    };
    expect(response.payload.length).toBe(3);
  });
});

describe("Status DTOs", () => {
  test("RelayStatus", () => {
    const status: RelayStatus = {
      id: 0,
      state: RelayState.ON,
    };
    expect(status.state).toBe(RelayState.ON);
  });

  test("FanStatus", () => {
    const status: FanStatus = {
      id: 0,
      speed: 75,
      mode: FanMode.AUTO,
      status: 0,
    };
    expect(status.speed).toBe(75);
  });

  test("SystemStatus", () => {
    const status: SystemStatus = {
      relays: [{ id: 0, state: RelayState.ON }],
      fans: [{ id: 0, speed: 50, mode: FanMode.MANUAL, status: 0 }],
      temperatures: [{ id: 0, temperature: 2500 }],
      drives: [{ id: 0, present: true }],
      leds: [
        {
          id: 0,
          red: 255,
          green: 0,
          blue: 0,
          brightness: 100,
          animation: LEDAnimation.IDLE,
        },
      ],
      timestamp: Date.now(),
    };
    expect(status.relays.length).toBe(1);
    expect(status.fans.length).toBe(1);
  });
});

describe("Configuration DTOs", () => {
  test("ConfigurationData", () => {
    const config: ConfigurationData = {
      version: 1,
      fanAutoMinSpeed: 10,
      fanAutoMaxSpeed: 100,
      tempWarningThreshold: 6000,
      tempCriticalThreshold: 7500,
      autoStartEnabled: true,
      debugMode: false,
    };
    expect(config.version).toBe(1);
    expect(config.tempWarningThreshold).toBe(6000);
  });
});

describe("Result Type", () => {
  test("Ok creates a success result", () => {
    const result: Result<number> = Ok(42);
    expect(result.success).toBe(true);
    if (result.success) {
      expect(result.value).toBe(42);
    }
  });

  test("Err creates a failure result", () => {
    const result: Result<number> = Err("TEST_ERROR", "Something went wrong");
    expect(result.success).toBe(false);
    if (!result.success) {
      expect(result.error.code).toBe("TEST_ERROR");
      expect(result.error.message).toBe("Something went wrong");
    }
  });

  test("Result can be used in control flow", () => {
    const testResult: Result<string> = Ok("success");

    let value = "";
    if (testResult.success) {
      value = testResult.value;
    }
    expect(value).toBe("success");
  });

  test("Failure result in control flow", () => {
    const testResult: Result<string> = Err("FAIL", "failed");

    let errorCode = "";
    if (!testResult.success) {
      errorCode = testResult.error.code;
    }
    expect(errorCode).toBe("FAIL");
  });
});

describe("Event Types", () => {
  test("all event type strings are defined", () => {
    expect(EventType.DEVICE_CONNECTED).toBeDefined();
    expect(EventType.RELAY_STATE_CHANGED).toBeDefined();
    expect(EventType.FAN_SPEED_CHANGED).toBeDefined();
    expect(EventType.COMMAND_SENT).toBeDefined();
    expect(EventType.ERROR_OCCURRED).toBeDefined();
  });

  test("event type values are unique", () => {
    const values = Object.values(EventType);
    const unique = new Set(values);
    expect(unique.size).toBe(values.length);
  });
});

describe("Contract Consistency", () => {
  test("no command codes conflict with response bits", () => {
    Object.values(CommandCode).forEach((cmd) => {
      expect(cmd & ResponseBit.ACK).toBe(0);
      expect(cmd & ResponseBit.NAK).toBe(0);
    });
  });

  test("packet structure is consistent", () => {
    const packet: Packet = {
      header: { magic: ProtocolInfo.Header },
      metadata: {
        sequence: 0,
        command: CommandCode.SYSTEM_PING,
        length: 0,
      },
      payload: new Uint8Array(0),
      footer: {
        crc16: 0x0000,
        terminator: ProtocolInfo.Footer,
      },
    };

    expect(packet.header.magic).toBe(ProtocolInfo.Header);
    expect(packet.footer.terminator).toBe(ProtocolInfo.Footer);
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
