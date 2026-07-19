# Security Policy

Security guidelines for a local-only NAS Controller application.

---

## Security Model

**Threat Model:** Local-only application
- **Users:** Trusted (same network, same device owner)
- **Attack Surface:** Minimal (localhost only)
- **Assumptions:** No malicious local users
- **Focus:** Robustness, not protection from attackers

**This is NOT:**
- Internet-facing application
- Multi-tenant system
- Customer-facing service

---

## Input Validation

### All User Input Must Be Validated

**Rule:** Every input from UI, files, or network is checked.

```typescript
// ✅ GOOD: Validate all input
function setRelaySpeed(id: number, speed: number) {
  if (!Number.isInteger(id)) throw new ValidationError("ID must be integer");
  if (id < 0 || id >= 4) throw new ValidationError("ID out of range");
  if (!Number.isInteger(speed)) throw new ValidationError("Speed must be integer");
  if (speed < 0 || speed > 100) throw new ValidationError("Speed 0-100");
  
  // Now safe to use
  sendCommand(id, speed);
}

// ❌ BAD: No validation
function setRelaySpeed(id, speed) {
  sendCommand(id, speed);  // What if speed is -999 or "DROP TABLE"?
}
```

### Validation Rules

| Input | Validate | Examples |
|-------|----------|----------|
| Relay ID | Integer, 0-3 | `if (!(id in [0,1,2,3]))` |
| Speed | Integer, 0-100 | `if (speed < 0 \|\| speed > 100)` |
| Port name | String, known pattern | `/dev/ttyUSB\d+` or `COM\d+` |
| Baudrate | Whitelist values | `[115200, 921600]` |
| Timeout | Integer, reasonable range | `1000-10000 ms` |
| Config value | Type + range + enum | Check type, bounds, allowed values |

### No SQL Injection Risk

N/A - this application doesn't use SQL.

### No Command Injection Risk

**Rule:** Never pass user input to `exec()` or `shell()`.

```typescript
// ❌ BAD: Command injection possible
const port = userInput.port;  // "/dev/ttyUSB0; rm -rf /"
exec(`stty -F ${port} 115200`);  // Dangerous!

// ✅ GOOD: Use library instead
const serial = new SerialPort({ path: port, baudRate: 115200 });
```

### No Arbitrary Code Execution

```typescript
// ❌ BAD: Arbitrary code execution
const config = JSON.parse(userInput);
eval(config.startup_script);  // NO NO NO

// ✅ GOOD: Only specific, safe operations
const config = JSON.parse(userInput);
setRelay(config.relay_id, config.value);  // Specific, validated
```

---

## File Operations

### Path Sanitization

**Rule:** User input paths are validated before use.

```typescript
// ❌ BAD: Path traversal possible
const logFile = `/logs/${userInput.date}.log`;
// User sends: "../../etc/passwd" → reads system file!

// ✅ GOOD: Whitelist allowed values
const logFile = `/logs/${sanitizeDateString(userInput.date)}.log`;
function sanitizeDateString(input) {
  if (!/^\d{4}-\d{2}-\d{2}$/.test(input)) throw new Error("Invalid date");
  return input;  // Now safe
}
```

### Config File Permissions

```
Config file: ~/.nas-controller/config.json
Permissions: 0600 (read/write owner only)
Reason: Contains sensitive settings
```

### Firmware Update Verification

```typescript
// ✅ GOOD: Verify before flashing
async function updateFirmware(filePath: string) {
  const file = fs.readFileSync(filePath);
  
  // Validate file
  if (!isValidFirmwareFormat(file)) throw new Error("Invalid firmware");
  if (!isValidFileSize(file)) throw new Error("Wrong size");
  // Optionally: verify signature if firmware is signed
  
  // Then flash
  await sendFirmware(file);
}

// ❌ BAD: No verification
async function updateFirmware(filePath: string) {
  const file = fs.readFileSync(filePath);
  await sendFirmware(file);  // Trust the file blindly
}
```

---

## Network Security

### WebSocket Only (No HTTP)

**Rule:** Real-time updates use WebSocket. No REST for state.

- WebSocket is persistent, encrypted (WSS)
- Harder to intercept or replay

### Localhost Binding Only

```typescript
// ✅ GOOD: Only listen on localhost
const server = express();
server.listen(3000, "127.0.0.1");  // Not 0.0.0.0

// ❌ BAD: Exposed to network
server.listen(3000, "0.0.0.0");  // Anyone can connect
```

### No Credentials in URLs

```typescript
// ❌ BAD: Password in URL
const url = "http://user:password@localhost:3000/api";

// ✅ GOOD: No auth needed (localhost-only)
const url = "http://localhost:3000";
```

---

## Configuration Security

### Sensitive Data

**Never log:**
- Passwords (none in this app)
- API keys (none in this app)
- Firmware binary data
- Personal information

**Safe to log:**
- Command types
- Status values
- Timing information
- Configuration keys (not values)

### Configuration Validation

```typescript
// ✅ GOOD: Validate all config values
const config = loadConfig();
if (!config.port) throw new Error("Port required");
if (!VALID_BAUDRATES.includes(config.baudRate)) {
  throw new Error("Invalid baudrate");
}
if (config.timeout < 100 || config.timeout > 60000) {
  throw new Error("Timeout out of range");
}

// ❌ BAD: Trust config blindly
const config = JSON.parse(fs.readFileSync("config.json"));
useConfig(config);  // What if timeout is -1?
```

---

## Rate Limiting

### Command Rate Limiting

**Rule:** Don't allow command flooding.

```typescript
const MAX_COMMANDS_PER_SECOND = 10;

function rateLimitCheck(clientId: string) {
  const now = Date.now();
  const windowStart = now - 1000;
  
  // Count commands in last second
  const recentCommands = commandLog.filter(
    c => c.clientId === clientId && c.time > windowStart
  );
  
  if (recentCommands.length >= MAX_COMMANDS_PER_SECOND) {
    throw new RateLimitError("Too many commands");
  }
}
```

### WebSocket Connection Limits

**Rule:** Limit concurrent connections.

```typescript
const MAX_CLIENTS = 5;

socket.on("connect", () => {
  if (getActiveConnections() >= MAX_CLIENTS) {
    socket.disconnect();
    return;
  }
  acceptClient(socket);
});
```

---

## Data Validation

### Packet Size Limits

```cpp
// ✅ GOOD: Enforce maximum size
const uint16_t MAX_PACKET_SIZE = 256;
if (packet.length > MAX_PACKET_SIZE) {
  return PACKET_TOO_LARGE;
}
```

### Configuration Size Limits

```typescript
const MAX_CONFIG_SIZE = 4096;  // 4KB max

function loadConfig(filePath: string) {
  const stats = fs.statSync(filePath);
  if (stats.size > MAX_CONFIG_SIZE) {
    throw new Error("Config file too large");
  }
  return JSON.parse(fs.readFileSync(filePath));
}
```

### Event Log Rotation

```typescript
const MAX_LOG_SIZE = 1000000;  // 1MB
const LOG_FILE = "events.log";

function rotateLogIfNeeded() {
  const stats = fs.statSync(LOG_FILE);
  if (stats.size > MAX_LOG_SIZE) {
    fs.renameSync(LOG_FILE, LOG_FILE + ".1");
    // Optionally delete old logs
  }
}
```

---

## Firmware Security

### Bootloader Verification

Firmware bootloader validates firmware integrity (outside scope of daemon).

### Over-the-Air Updates

```cpp
// ✅ GOOD: Verify before writing to flash
if (!verifyFirmwareSignature(data)) {
  return UPDATE_FAILED;
}
// Flash only after verification passes
writeFirmwareToFlash(data);
```

### Config Storage

Stored in flash (encrypted by ESP32 security features).

---

## Logging & Monitoring

### What Gets Logged

**ERROR level:**
- Command failed
- Connection lost
- Validation error
- Firmware exception

**WARN level:**
- Retry needed
- Unusual state
- Configuration changed
- Deprecated API used

**INFO level:**
- Device connected
- Status changed
- Configuration saved
- User action (if local only)

**DEBUG level:**
- Command queued
- Packet sent/received (without sensitive data)
- State transitions

### What Never Gets Logged

- Firmware binary data
- Passwords or secrets
- Full packets (log headers only)
- Personal information

---

## Checklist Before Release

Security review before deploying:

- [ ] No hardcoded secrets (API keys, passwords)
- [ ] All user input validated
- [ ] No `eval()` or `exec()` with user input
- [ ] File paths sanitized
- [ ] Localhost binding only
- [ ] No credentials in URLs or configs
- [ ] Rate limiting implemented
- [ ] Packet size limits enforced
- [ ] Error messages don't leak info
- [ ] Dependencies checked for vulnerabilities
- [ ] No timing attacks possible
- [ ] Firmware updates verified before flashing

---

## Dependency Security

### Update Dependencies Regularly

```bash
npm audit          # Check for vulnerabilities
npm update         # Update to latest safe version
npm outdated       # See what's outdated
```

### Pin Dependency Versions

```json
{
  "dependencies": {
    "express": "4.18.2",  // ✅ Exact version (pinned)
    "socket.io": "~4.5.0"  // ✅ Minor updates only
  }
}
```

---

## Security Philosophy

**For this application:**
- Trust local users (same network/device)
- Validate all input anyway (robustness)
- Keep secrets out of logs
- Limit resource consumption (prevent DoS)
- No authentication needed (localhost-only)
- No encryption needed (private network)

**Not a concern:**
- Protecting against skilled attackers
- Hiding state from local users
- Multi-tenant isolation
- Internet-scale scalability

---

## Questions to Ask

Before committing code:

1. **Can user input break the code?** (Validate)
2. **Could this run untrusted code?** (Never exec user input)
3. **Is sensitive data leaked?** (Don't log secrets)
4. **Could someone DoS this?** (Rate limit)
5. **Are dependencies safe?** (Audit regularly)

If you answer "maybe" to any, fix it before committing.

---

**Security is everyone's responsibility. Default to defensive coding.**
