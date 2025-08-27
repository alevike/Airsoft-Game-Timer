# Airsoft-Timer---Open-Source-version

This repo is under construction!

# Airsoft Game Timer

## Overview
This project is an Arduino-based Airsoft Game Timer. It supports multiple game modes and is designed for DIY use. Commercial use is prohibited (see LICENSE).

## Uploading Firmware to the Timer Board (*or Arduino Mega)
You can upload the latest firmware to your Airsfot Timer board via FTDI adapter using `avrdude` and the wiring protocol. This method preserves the bootloader.

### Requirements
- **Timer Board (*or Arduino Mega)**
- **ISP programmer or Arduino as ISP**
- **avrdude** (AVR Downloader/UploaDEr)
- **Latest firmware HEX file** (from the Releases section)

### Download avrdude
- Official site: [https://www.nongnu.org/avrdude/](https://www.nongnu.org/avrdude/)
- Windows users: Download the ZIP from [https://github.com/avrdudes/avrdude/releases](https://github.com/avrdudes/avrdude/releases)
- Extract the ZIP to a folder, e.g. `C:\Users\YourName\Desktop\avrdude`

### Download the Latest Firmware
1. Go to the [Releases](https://github.com/alevike/Airsoft-Game-Timer/releases) page of this repository.
2. Download the latest `firmware.hex` file.
3. Place `firmware.hex` in the same folder as `avrdude.exe`.

### Upload Procedure
1. Connect your Board to your PC via ISP (or use Arduino as ISP).
2. Open a command prompt or PowerShell in the folder where `avrdude.exe` and `firmware.hex` are located.
3. Run the following command (replace `COM12` with your actual port):

```powershell
./avrdude.exe -D -c wiring -p m2560 -P COM12 -b 115200 -U flash:w:firmware.hex:i
```

- `-D` disables chip erase (preserves bootloader)
- `-c wiring` uses the wiring/Arduino ISP protocol
- `-p m2560` specifies the Mega 2560 chip
- `-P COM12` is your serial port (check Device Manager)
- `-b 115200` is the baud rate
- `-U flash:w:firmware.hex:i` writes the firmware

### Notes
- Make sure your programmer and wiring are correct.
- If you get errors, try lowering the baud rate (e.g., `-b 19200`).
- The bootloader will not be erased with this method.

## Hardware Compatibility

This software is intended to be used with a specially designed Airsoft Game Timer board, available soon on Tindie. 

**Tindie link placeholder:** [Tindie Store - Airsoft Game Timer Board](https://www.tindie.com/products/your-product-id/)

The firmware can also be adapted for use with a standard Arduino Mega 2560 board, but some custom features (such as battery monitoring and soft power-off) will not be available.

### Pin Connections (Base Arduino Mega 2560)

| Module         | Pin(s)         | Notes                       |
|----------------|----------------|-----------------------------|
| LCD module     | rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2 | No i2c backapck required |
| Buzzer         | 25             | Digital output              |
| NFC Module     | IRQ: 6, RESET: 7 | Standard i2c pins + IRQ/RESET   |
| LoRa Module    | AUX: 22, M0: 23, M1: 24 | Serial2 + custom pins |
| Keyboard (Keypad) | Rows: 37,36,35,34<br>Cols: 33,32,31,30 | 4x4 matrix keypad |
| LED Pixel (WS2812) | DATA: 23, Clock: 24      | NeoPixel/WS2812 strip/Apa102..     |
| Illuminated button leds | Red: 28, Green: 29 | | 
| Wire sensing pins | Wire1: 49 , Wire2: 48, Wire3: 47, Wire4: 46, Commond: GND or 45| used in the "Cut the wire game mode" |

For full pin mapping and advanced features, refer to the source code.

## License
This project is licensed under CC BY-NC 4.0. See LICENSE for details.
