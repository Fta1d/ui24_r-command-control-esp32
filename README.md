# UI24R Command Control ESP32

This project provides a solution for controlling a Soundcraft Ui24R digital mixer using an ESP32 microcontroller with ESP-IDF framework, allowing for customizable remote control capabilities.

## Overview

The UI24R Command Control ESP32 project enables wireless control of Soundcraft Ui24R digital mixers via a custom ESP32-based controller. It implements communication protocols to interact with the mixer's API, allowing control of various mixer functions through physical buttons and controls connected to the ESP32. Built using the native ESP-IDF framework for maximum performance and flexibility.

## Features

- Connect to the Ui24R mixer via WiFi
- Control mixer functions remotely
- Trigger scene changes with physical buttons
- Monitor mixer status via ESP32
- Customizable control interface
- Low-latency communication with the mixer
- Built with ESP-IDF for optimal performance

## Hardware Requirements

- ESP32 development board
- Tactile push buttons (quantity depends on your configuration)
- Resistors and jumper wires
- Power supply for ESP32
- Optional: Enclosure for your control system

## Software Dependencies

- ESP-IDF (Espressif IoT Development Framework)
- cJSON library (included in ESP-IDF)
- ESP32 WebSocket component
- Development environment with ESP-IDF installed (Windows, Linux, or macOS)

## Installation

1. Clone this repository to your local machine:
   ```
   git clone https://github.com/Fta1d/ui24_r-command-control-esp32.git
   ```

2. Set up ESP-IDF environment according to [official documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html)

3. Navigate to the project directory:
   ```
   cd ui24_r-command-control-esp32
   ```

4. Configure the project:
   ```
   idf.py menuconfig
   ```
   - Navigate to "UI24R Control Configuration" to set WiFi credentials and mixer settings

5. Build the project:
   ```
   idf.py build
   ```

6. Flash the firmware to your ESP32:
   ```
   idf.py -p [PORT] flash
   ```
   Where [PORT] is your ESP32's serial port (e.g., COM3 on Windows or /dev/ttyUSB0 on Linux)

## Configuration

Configure the project using ESP-IDF's configuration system:

```
idf.py menuconfig
```

Navigate to the project's custom configuration section to set:
- WiFi credentials
- Ui24R mixer IP address and port
- Button GPIO pin assignments
- Other control parameters

Alternatively, you can directly edit the `sdkconfig.defaults` file or create a custom configuration:

```
# WiFi Settings
CONFIG_UI24R_WIFI_SSID="your_wifi_ssid"
CONFIG_UI24R_WIFI_PASSWORD="your_wifi_password"

# Ui24R Mixer Settings
CONFIG_UI24R_MIXER_IP="192.168.1.X"
CONFIG_UI24R_MIXER_PORT=80

# Button GPIO Pin Assignments
CONFIG_UI24R_BUTTON_SCENE_1=12
CONFIG_UI24R_BUTTON_SCENE_2=14
CONFIG_UI24R_BUTTON_MUTE_MASTER=27
```

## Wiring

Connect your buttons to the ESP32 as defined in your project configuration. A typical button connection requires:

1. Connect one side of each button to the corresponding GPIO pin
2. Connect the other side of each button to ground (GND)
3. Use pull-up resistors (10kΩ) between the GPIO pin and 3.3V

## Usage

1. Power on your Ui24R mixer and ensure it's connected to your network
2. Power on the ESP32 controller
3. The ESP32 will automatically connect to the configured WiFi network
4. Once connected, the ESP32 will establish a connection with the Ui24R mixer
5. Press the configured buttons to control various mixer functions
6. Monitor console output via `idf.py monitor` for debugging information

## Communication Protocol

This project uses the Soundcraft Ui24R's WebSocket API to send commands and receive status updates. Commands are sent in JSON format, following the mixer's API specification.

Example command to recall a scene:
```json
{"sceneRecall":3}
```

## Project Structure

The main functionality is implemented in the ESP-IDF project structure:

- `main/main.c`: Entry point and application task management
- `main/ui24r_api.c`: API communication with the Ui24R mixer
- `main/button_handler.c`: Physical button input handling using ESP-IDF's GPIO API
- `main/websocket_client.c`: WebSocket client implementation
- `main/Kconfig.projbuild`: Project configuration options
- `CMakeLists.txt`: Project build system definition

## Troubleshooting

- **ESP32 won't connect to WiFi**: Verify your SSID and password in menuconfig settings
- **Cannot connect to mixer**: Check that the mixer's IP address is correct and that it's on the same network
- **Buttons not responding**: Verify your wiring and GPIO pin assignments
- **Commands not being received by mixer**: Ensure the WebSocket connection is established properly
- **Build errors**: Make sure you've installed all necessary ESP-IDF components by running `idf.py add-dependency [component-name]`
- **Flash fails**: Check your USB connection and serial port settings

For detailed debugging:
```
idf.py monitor
```

## Future Enhancements

- LCD display support for status feedback
- Additional control elements (faders, rotary encoders)
- Multi-mixer support
- Custom MIDI control surface implementation
- Scene management system
- OTA (Over-The-Air) firmware updates

## Contributing

Contributions to this project are welcome! Please feel free to submit pull requests or open issues to suggest improvements or report bugs.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Soundcraft Ui24R development team
- ESP32 community and Espressif Systems
- Contributors to the ESP-IDF framework
