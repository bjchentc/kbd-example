# ⌨️ Keyboard Example Project

## 🌟 Overview

The Keyboard Example Project is a Zephyr-based application designed to demonstrate keyboard scanning and event handling for an embedded system with a matrix keyboard. It utilizes the Zephyr RTOS input subsystem to process keyboard events and includes an optional event-driven thread for handling keyboard input asynchronously.

### 🎯 Key Features

- Matrix keyboard scanning with configurable row and column sizes
- Event-driven keyboard input processing
- Power management (PM) features for the keyboard device
- Comprehensive logging for debugging and monitoring keyboard events

---

## 🔧 Hardware Requirements

- **Development Board**: Zephyr-compatible board with matrix keyboard interface
- **Debug Console**: UART2 configured at 115200 baud rate
- **Memory**: SRAM with at least 128 KB of DLM (Data Local Memory)
- **Keyboard**: Matrix keyboard compatible with your target board

---

## 📦 Software Dependencies

- **Zephyr RTOS**: Compatible version with provided `CMakeLists.txt`
- **Input Subsystem**: `CONFIG_INPUT` enabled
- **Power Management**: `CONFIG_PM` enabled
- **Event Subsystem**: `CONFIG_EVENTS` enabled
- **Logging Subsystem**: `CONFIG_LOG` enabled

---

## 📁 Project Structure

```
kb_example/
├── src/
│   ├── main.c          # Main application code with keyboard handling
│   └── main.h          # Header file with event types and declarations
├── app.overlay         # Device tree overlay for hardware configuration
├── Kconfig            # Custom configuration options
├── CMakeLists.txt     # Zephyr build configuration
├── prj.conf           # Project configuration settings
└── README.md          # This documentation
```

### 📝 File Descriptions

- **`src/main.c`**: Main application code, including keyboard initialization, input callback handling, and optional event thread
- **`src/main.h`**: Header file defining event types and keyboard initialization function
- **`app.overlay`**: Device tree overlay configuring SRAM, UART2, and keyboard properties
- **`Kconfig`**: Configuration options for enabling debug logging and event thread hooking
- **`CMakeLists.txt`**: Build configuration for the Zephyr project
- **`prj.conf`**: Project configuration enabling input, power management, events, and logging

---

## 🚀 Build Instructions

### 🛠️ 1. Set up Zephyr Environment

Ensure the Zephyr RTOS environment is set up and set the `ZEPHYR_BASE` environment variable:

```bash
export ZEPHYR_BASE=/path/to/zephyr
```

### 📥 2. Clone the Repository

Clone this project to your local machine:

```bash
git clone <repository-url>
cd kb_example
```

### 🔨 3. Build the Project

Use the Zephyr build system (west) to build the project for your target board:

```bash
west build -b <your-board-name> .
```

### ⚡ 4. Flash the Firmware

Flash the compiled firmware to your board:

```bash
west flash
```

---

## ⚙️ Configuration Options

The project includes the following Kconfig options (defined in `Kconfig`):

### 🐛 `EC_KBD_DBG`

- **Purpose**: Enables debug logging for keyboard events and initialization
- **Default**: Enabled (`CONFIG_EC_KBD_DBG=y` in `prj.conf`)
- **Usage**: Provides detailed logging for troubleshooting

### 🔄 `EC_KBD_HOOK_EVENT_THREAD`

- **Purpose**: Enables an event-driven thread for handling keyboard input asynchronously
- **Default**: Enabled (`CONFIG_EC_KBD_HOOK_EVENT_THREAD=y` in `prj.conf`)
- **Usage**: Processes keyboard events in a separate thread with 5ms processing time

> 💡 **Tip**: Modify these options in `prj.conf` to enable or disable features as needed.

---

## 🌳 Device Tree Configuration

The `app.overlay` file configures the following hardware components:

### 💾 SRAM Configuration

- **Size**: 128 KB DLM (Data Local Memory)
- **Address**: `0x800000`

### 🔌 UART2 Configuration

- **Purpose**: Debug console
- **Baud Rate**: 115200
- **Clock Frequency**: 1.8432 MHz

### ⌨️ Matrix Keyboard Configuration

- **Device**: `&kbd`
- **Matrix Size**: 8x16 (8 rows × 16 columns)
- **Customization**: Configurable via `row-size` and `col-size` properties

> 🔧 **Customization**: Edit `app.overlay` to match your specific hardware setup.

---

## 🎮 Usage

### 🔗 1. Connect the Hardware

- Ensure your development board is connected to a matrix keyboard
- Connect UART console for debugging output

### 🏃 2. Run the Application

After flashing, the application will:

- Initialize the keyboard subsystem
- Start scanning for input events
- Begin processing keyboard input

### 📊 3. Monitor Output

Use a serial terminal (e.g., `minicom`, `screen`, or `PuTTY`) to monitor UART2 output at 115200 baud.

#### 📝 Example Log Output

If `EC_KBD_DBG` is enabled, you'll see output similar to:

```
[00:00:00.123,456] <inf> main: Keyboard Example Enabled.
[00:00:00.123,789] <inf> main: Keyboard Initialization Complete
[00:00:01.234,567] <inf> main: Keyboard Event Triggered: [x, y, pressed] = [2, 3, 1]
```

### 🎯 4. Keyboard Events

The application detects:

- **Key Presses**: `INPUT_BTN_TOUCH` events
- **Row Events**: `INPUT_ABS_Y` for row detection
- **Column Events**: `INPUT_ABS_X` for column detection

If `EC_KBD_HOOK_EVENT_THREAD` is enabled, an event thread processes keyboard input with a 5ms emulated processing time.

---

## ⚠️ Important Notes

### ✅ Prerequisites

- Ensure the keyboard device is properly connected and configured in the device tree
- Verify all hardware connections before powering on

### 🐛 Troubleshooting

- If the keyboard device is not ready, an error message will be logged (when `EC_KBD_DBG` is enabled)
- Check device tree configuration if keyboard events are not detected

### 🔧 Configuration Tips

- Default matrix size is 8 rows × 16 columns, adjustable in `app.overlay`
- Power management is enabled by default to optimize keyboard operation
- Logging level can be adjusted in `prj.conf` for different verbosity levels

---

## 📄 License

**Copyright © 2025. All Rights Reserved.**

Licensed under the Apache License, Version 2.0. See the SPDX-License-Identifier in the source files for details.

---

## 👨‍💻 Support

For issues, questions, or contributions, please refer to the project repository or contact the development team.

---

**Author:** Benjamin Joseph Chen  
**Email:** [bjchentc@gmail.com](mailto:bjchentc@gmail.com)
