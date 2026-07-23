# Not Planned For Frozen Firmware API

The current firmware does not implement the following features and documentation must not describe them as available:

- ASCII serial command protocol.
- Firmware update over the NAS controller protocol.
- File transfer over the NAS controller protocol.
- Debug console over the public protocol.
- Authentication or encryption.
- Network transport.
- Packet fragmentation/reassembly.
- Protocol negotiation.
- Endian-independent wire encoding.
- Asynchronous event packets pushed independently of command responses.
- Implemented `LedGet` or `LedSetColor` behavior.
