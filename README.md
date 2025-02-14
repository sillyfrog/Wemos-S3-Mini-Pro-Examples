# Wemos S3 Mini Pro v1.1.0 Examples

A few [PlatformIO](https://platformio.org/)/[pioarduino](https://github.com/pioarduino/) examples for the [Wemos S3 Mini Pro](https://www.wemos.cc/en/latest/s3/s3_mini_pro.html) v1.1.0 board.

When looking for examples that worked with PlatformIO, I found most had at least one issue that meant it did not work at all, or it was broken in some way.

To build these I'm using the [pioarduino VSCodium extension](https://marketplace.visualstudio.com/items?itemName=pioarduino.pioarduino-ide). The rest of the required configuration is in the `platformio.ini` file.

## Examples

Each directory has it's own README.md with a bit more information about the example and what was wrong with some of the other examples I found.

Each example should be opened from the directory in VSCode, _not_ from the root of this repository.

### IMU Demo

This uses the onboard _6D MEMS IMU (QMI8658C)_ to display the orientation of the board on the serial monitor, and change the color of the onboard RGB LED based on the orientation.

### RGB Blink

A minimal example to show and change the color of the onboard RGB LED and write to the serial port.

### TFT Demo

An example (built from the upstream library example) that displays a few different things on the onboard TFT display in a loop.