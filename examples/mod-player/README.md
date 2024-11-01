# MOD Player Example for ESP32

This example demonstrates how to use the `esp32-mod-player` component to enable MOD file playback on ESP32 devices.

## Overview

The example leverages the `esp32-mod-player` component, which integrates the [ModPlayer](https://github.com/realsba/ModPlayer) library as a submodule to provide efficient MOD file playback capabilities for ESP32.

## Requirements

- ESP32 device
- ESP-IDF v5.4 or later
- `esp32-mod-player` component (included in the main repository)

## Setup

1. **Build the example** in the main project directory:

    ```bash
    idf.py build
    ```

2. **Flash the example** to your ESP32 device:

    ```bash
    idf.py -p /dev/ttyUSB0 flash monitor
    ```

   > Replace `/dev/ttyUSB0` with the port your ESP32 is connected to.

3. MOD Files:

The example includes a `mod` subdirectory containing MOD files sourced from [modarchive.org](https://modarchive.org/). This directory is used to demonstrate MOD playback functionality.

To load the MOD files onto your ESP32, create and flash a SPIFFS partition using the following commands:

```bash
python $IDF_PATH/components/spiffs/spiffsgen.py 131072 mod spiffs.bin
python $IDF_PATH/components/esptool_py/esptool/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 115200 write_flash -z 0x200000 spiffs.bin
``` 

## Usage

After flashing, the example will automatically start MOD playback using the `esp32-mod-player` component. Monitor the serial output for playback status and debug information.

## Notes

- This example is part of the `esp32-mod-player` repository. For details on configuring `ModPlayer`, consult the library’s documentation.

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/realsba/esp32-mod-player/blob/main/LICENSE) file for details.

## Author

- Bohdan Sadovyak
