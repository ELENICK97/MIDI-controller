# DoxMidiController

A DIY USB MIDI controller built on an **Arduino Leonardo**, with a custom shield PCB designed in KiCad.

It has 9 slide faders, 8 rotary knobs, a 2-axis joystick (modulation wheel and pitch bend), 8 latching buttons and 8 momentary push buttons. It also has a **5-pin DIN MIDI IN** port, and whatever arrives there is forwarded to the computer over USB. So the board also works as a simple MIDI-to-USB interface for an external keyboard.

The faders are meant to be used like Hammond-style drawbars.
<img width="1511" height="1146" alt="FinalBoard" src="https://github.com/user-attachments/assets/7f3ee1e1-0b65-41ed-80f0-1b9349a3b6aa" />

## Features

| Control | Qty | Part | MIDI message |
|---|---|---|---|
| Slide faders (RV1–RV9) | 9 | Bourns PTL60-15G1-103B2, 10 kΩ, 60 mm travel | CC 50–58 |
| Rotary knobs (RV10–RV17) | 8 | Alps RK09L, 10 kΩ | CC 59–66 (inverted in software) |
| Joystick, horizontal axis | 1 | SparkFun COM-09032 | CC 1 (modulation wheel) |
| Joystick, vertical axis | 1 | SparkFun COM-09032 | Pitch bend |
| Latching buttons (S1–S8) | 8 | GPBS-850N | CC 67–74 |
| Momentary push buttons (SW1–SW8) | 8 | 6 mm tactile switch | CC 75–82 |
| MIDI IN | 1 | DIN-5 jack (SparkFun PRT-09536) + 6N138 optocoupler | Forwarded to USB |

All messages are sent on **MIDI channel 1**. The controller shows up on the computer as a class-compliant USB MIDI device, so no driver is needed.

## Repository layout

```
firmware/
  2024_MIDI_controller_control_surface/
    2024_MIDI_controller_control_surface.ino   Arduino sketch
hardware/
  kicad/       KiCad 6 project (schematic, PCB layout, project file)
  gerbers/     Manufacturing files (Gerbers + drill files, "v1" revision)
LICENSE        GNU GPL v3
```

## Hardware

The PCB is a shield that plugs onto an Arduino Leonardo (it uses the Uno R3 header layout). Four **CD4051 / CD74HC4051** 8-channel analog multiplexers let the Leonardo read all 33 controls with only a few pins:

| Mux | Reads | Signal pin | Select pins S0 / S1 / S2 |
|---|---|---|---|
| U5 | RV5–RV9, RV14–RV16 | A0 | D13 / D12 / D11 |
| U2 | RV1–RV4, RV10–RV13 | A1 | D6 / D5 / D4 |
| U3 | S1–S4, SW1–SW4 | D7 | D3 / D2 / D1 |
| U4 | S5–S8, SW5–SW8 | A5 | D8 / D9 / D10 (see the known issue below) |

These controls are wired straight to the Leonardo: RV17 → A2, joystick H → A3, joystick V → A4. The MIDI IN optocoupler output goes to D0 (RX).

To open the design, use **KiCad 6 or later** (newer versions will offer to convert the files). The symbols are stored inside the schematic and the footprints inside the PCB file, so you don't need any extra libraries to open, view or edit the board. Some third-party 3D models are not included (see [Credits](#credits)), so a few parts are missing from the 3D viewer.

To have the board made, zip the contents of [`hardware/gerbers/`](hardware/gerbers/) and upload the zip to a PCB manufacturer.

## Firmware

The firmware uses the [Control Surface](https://github.com/tttapa/Control-Surface) library.

### Build and upload

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Install the **Control Surface** library (version 2.x) from <https://github.com/tttapa/Control-Surface>. Download the ZIP, then use *Sketch → Include Library → Add .ZIP Library…*. If the compiler asks for it, also install **MIDIUSB** from the Library Manager.
3. Open `firmware/2024_MIDI_controller_control_surface/2024_MIDI_controller_control_surface.ino`.
4. Select *Tools → Board → Arduino Leonardo* and the correct port, then click Upload.

### Customising

The CC numbers are defined as `#define`s at the top of the sketch, so you can change them there. The `invert()` loop in `setup()` sets which potentiometers are reversed.

## Known issues

- **Absolute include path.** The sketch includes `SoftwareSerialMIDI_Interface.hpp` through an absolute Windows path (`C:\Users\xxxxxx\Documents\Arduino\libraries\...`). On any other computer, change that line to:
  ```cpp
  #include <MIDI_Interfaces/SoftwareSerialMIDI_Interface.hpp>
  ```
- **D0 and D8 are swapped between the PCB and the firmware.** On the PCB (v1), MIDI IN goes to **D0** and U4's S0 select line goes to **D8**. The firmware does the opposite: it reads MIDI IN with `SoftwareSerial` on **D8** and drives U4's S0 from **D0** (`mux_SW2 {0, 9, 10}`). The firmware only works if those two connections are swapped on the board, for example by cutting the traces and adding jumper wires. Otherwise, change the firmware to `mux_SW2 {8, 9, 10}` and read MIDI IN with `HardwareSerialMIDI_Interface` on `Serial1`.

## Credits

This project builds on work by others:

- **[Control Surface](https://github.com/tttapa/Control-Surface)** by Pieter P (tttapa). This Arduino library does all the MIDI, multiplexer, potentiometer and button handling in the firmware. Licensed under GPL-3.0.
- **[Arduino](https://www.arduino.cc/)**: the Arduino Leonardo board, Arduino core and `SoftwareSerial` library (LGPL-2.1). The shield footprint comes from the KiCad library.
- **[KiCad](https://www.kicad.org/)** and the official KiCad symbol, footprint and 3D model libraries (CC-BY-SA 4.0 with an exception for designs that use them), used for the schematic and PCB.
- **[SamacSys / Component Search Engine](https://componentsearchengine.com/)** provided the symbols, footprints and 3D models for the 6N138, GPBS-850N, PTL60-15G1-103B2 and PRT-09536 parts. Their licence doesn't allow redistributing these models as PCB library components, so the library files are **not** in this repository. The board design that uses them has no restrictions. Download the original models from Component Search Engine if you need them.
- **[SnapEDA](https://www.snapeda.com/)** provided the symbol and footprint for the SparkFun COM-09032 thumb joystick. Its original library files aren't included either.
- **[SparkFun Electronics](https://www.sparkfun.com/)** makes the thumb joystick (COM-09032) and the MIDI DIN connector (PRT-09536).
- The MIDI IN circuit (6N138 optocoupler, 220 Ω resistors, 1N914 protection diode) follows the standard opto-isolated input from the [MIDI 1.0 Electrical Specification](https://midi.org/specs) by the MIDI Association.

## License

Copyright © 2024 ElectroNick

This project (firmware and hardware design files) is released under the **GNU General Public License v3.0**. See [LICENSE](LICENSE). GPL-3.0 was chosen to match the Control Surface library the firmware is built on.
