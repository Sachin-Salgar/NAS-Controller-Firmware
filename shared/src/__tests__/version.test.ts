import {
  ProtocolVersion,
  ProjectVersion,
  CompatibilityVersion,
} from "../version";

describe("ProtocolVersion", () => {
  test("version numbers are defined", () => {
    expect(ProtocolVersion.MAJOR).toBe(1);
    expect(ProtocolVersion.MINOR).toBe(0);
    expect(ProtocolVersion.PATCH).toBe(0);
  });

  test("toString() returns semantic version", () => {
    expect(ProtocolVersion.toString()).toBe("1.0.0");
  });
});

describe("ProjectVersion", () => {
  test("version numbers are defined", () => {
    expect(ProjectVersion.MAJOR).toBe(1);
    expect(ProjectVersion.MINOR).toBe(0);
    expect(ProjectVersion.PATCH).toBe(0);
  });

  test("toString() returns semantic version", () => {
    expect(ProjectVersion.toString()).toBe("1.0.0");
  });
});

describe("CompatibilityVersion", () => {
  test("min protocol version is defined", () => {
    expect(CompatibilityVersion.MinProtocolVersion.MAJOR).toBe(1);
    expect(CompatibilityVersion.MinProtocolVersion.MINOR).toBe(0);
    expect(CompatibilityVersion.MinProtocolVersion.PATCH).toBe(0);
  });

  test("max protocol version is defined", () => {
    expect(CompatibilityVersion.MaxProtocolVersion.MAJOR).toBe(1);
    expect(CompatibilityVersion.MaxProtocolVersion.MINOR).toBe(99);
  });

  test("isCompatible returns true for exact version match", () => {
    expect(CompatibilityVersion.isCompatible(1, 0, 0)).toBe(true);
  });

  test("isCompatible returns true for minor version increase", () => {
    expect(CompatibilityVersion.isCompatible(1, 1, 0)).toBe(true);
  });

  test("isCompatible returns true for patch version increase", () => {
    expect(CompatibilityVersion.isCompatible(1, 0, 5)).toBe(true);
  });

  test("isCompatible returns false for major version increase", () => {
    expect(CompatibilityVersion.isCompatible(2, 0, 0)).toBe(false);
  });

  test("isCompatible returns false for version below minimum", () => {
    expect(CompatibilityVersion.isCompatible(0, 9, 9)).toBe(false);
  });

  test("isCompatible allows minor versions within range", () => {
    expect(CompatibilityVersion.isCompatible(1, 50, 0)).toBe(true);
  });

  test("isCompatible rejects minor versions beyond max", () => {
    expect(CompatibilityVersion.isCompatible(1, 100, 0)).toBe(false);
  });
});
