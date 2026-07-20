// ============================================================================
// VERSION DEFINITIONS
// ============================================================================
// Centralized versioning for protocol, project, and compatibility.
// All components import from here to maintain version consistency.

// PROTOCOL VERSIONING
// Increment when protocol format changes
// Major: Breaking changes (packet structure, command codes)
// Minor: Non-breaking additions (new commands, new fields at end)
// Patch: Bug fixes in existing behavior
export const ProtocolVersion = {
  MAJOR: 1,
  MINOR: 0,
  PATCH: 0,
  toString(): string {
    return `${this.MAJOR}.${this.MINOR}.${this.PATCH}`;
  },
} as const;

// PROJECT VERSIONING
// Increment project version independently of protocol
// Useful for tracking daemon/firmware/UI versions
export const ProjectVersion = {
  MAJOR: 1,
  MINOR: 0,
  PATCH: 0,
  toString(): string {
    return `${this.MAJOR}.${this.MINOR}.${this.PATCH}`;
  },
} as const;

// COMPATIBILITY VERSIONING
// Used to check if firmware and daemon can communicate
// If firmware protocol version < min required, fail with helpful message
export const CompatibilityVersion = {
  // Minimum protocol version daemon supports
  MinProtocolVersion: {
    MAJOR: 1,
    MINOR: 0,
    PATCH: 0,
  },
  // Maximum protocol version daemon can handle
  MaxProtocolVersion: {
    MAJOR: 1,
    MINOR: 99,  // Allow non-breaking additions
    PATCH: 99,
  },
  // Check if firmware version is compatible
  isCompatible(major: number, minor: number, patch: number): boolean {
    const minOk = major > this.MinProtocolVersion.MAJOR ||
      (major === this.MinProtocolVersion.MAJOR && minor >= this.MinProtocolVersion.MINOR);
    const maxOk = major < this.MaxProtocolVersion.MAJOR ||
      (major === this.MaxProtocolVersion.MAJOR && minor <= this.MaxProtocolVersion.MINOR);
    return minOk && maxOk;
  },
} as const;
