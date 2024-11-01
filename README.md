# ESP32 MOD Player Component

The `esp32-mod-player` is an ESP-IDF component designed to enable MOD file playback on ESP32 devices using the [ModPlayer](https://github.com/realsba/ModPlayer) library as a submodule.

## Features

- Provides seamless MOD file playback on ESP32 with minimal setup
- Built as an esp-idf component, making it compatible with the ESP-IDF build system
- Utilizes the `ModPlayer` library without additional code, ensuring efficient integration

## Requirements

- ESP-IDF v5.4 or later
- ModPlayer library (included as a submodule)

## Installation

To include `esp32-mod-player` in your project:

1. Use the `idf.py add-dependency` command to add the component from the ESP Component Registry:

    ```bash
    idf.py add-dependency "realsba/esp32-mod-player"
    ```

2. Alternatively, add the component manually as a Git submodule:

    ```bash
    git submodule add https://github.com/realsba/esp32-mod-player components/esp32-mod-player
    git submodule update --init --recursive
    ```

3. Ensure that the `ModPlayer` library is correctly initialized as a submodule.

## Configuration

No specific configuration is required for this component. However, check the `ModPlayer` documentation for any settings or tuning parameters.

## Usage

A sample usage example is available in the [`examples/mod-player`](examples/mod-player) folder. This example demonstrates how to set up MOD playback within an esp-idf project.

## Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/realsba/esp32-mod-player/blob/main/LICENSE) file for details.

## Author

- Bohdan Sadovyak

## Bugs/Issues

Please report any bugs or issues [here](https://github.com/realsba/esp32-mod-player/issues)
