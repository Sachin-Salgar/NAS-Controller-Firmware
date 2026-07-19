# Frontend Documentation

This folder contains all React/Vite web interface documentation.

## Core Documentation

- **[WEB_INTERFACE_PLAN.md](WEB_INTERFACE_PLAN.md)** - Feature roadmap and architecture
- **[UI_GUIDELINES.md](UI_GUIDELINES.md)** - Design system and UI patterns
- **[COMPONENTS.md](COMPONENTS.md)** - Component catalog and usage
- **[WEBSOCKET_CLIENT.md](WEBSOCKET_CLIENT.md)** - WebSocket client integration

## Pages & Features

- **[DASHBOARD.md](DASHBOARD.md)** - Dashboard page design and implementation
- **[CONTROLS.md](CONTROLS.md)** - Control panel (relays, fans, LEDs)
- **[TEST_CONSOLE.md](TEST_CONSOLE.md)** - Test console and packet inspector
- **[CONFIGURATION_EDITOR.md](CONFIGURATION_EDITOR.md)** - Config editor implementation
- **[STATISTICS.md](STATISTICS.md)** - Statistics and event logging page

## State Management

- **[STATE_MANAGEMENT.md](STATE_MANAGEMENT.md)** - Frontend state synchronization with daemon
- **[HOOKS.md](HOOKS.md)** - React hooks for WebSocket and state subscriptions
- **[SERVICES.md](SERVICES.md)** - API and WebSocket service layer

## Quick Links

- **Source Code:** `../src/`
- **Public Assets:** `../public/`
- **Configuration:** `../package.json`

## Contributing to Frontend

Before modifying frontend code, read:
1. [WEB_INTERFACE_PLAN.md](WEB_INTERFACE_PLAN.md)
2. [UI_GUIDELINES.md](UI_GUIDELINES.md)
3. [COMPONENTS.md](COMPONENTS.md)
4. Project-level [DEVELOPER_GUIDE.md](../../docs/DEVELOPER_GUIDE.md)

## Project Integration

This frontend is part of the NAS Controller project. See [Project Overview](../../docs/README.md) for system-wide documentation.

## Building & Running

```bash
cd frontend
npm install
npm run dev        # Development server with hot reload
npm run build      # Build for production
npm run preview    # Preview production build
```
