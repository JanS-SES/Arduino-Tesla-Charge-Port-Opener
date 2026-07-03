# Arduino Tesla Charge Port Opener

Minimal Arduino-compatible transmitter sketches for opening the Tesla charge port with a cheap 433 MHz OOK/ASK RF module.

The project replays a Tesla charge port opener bitstream captured with a LabNation SmartScope logic analyzer by bit-banging a GPIO pin. The main sketch sweeps the pulse length around the nominal 400 us timing, which avoids manual per-board calibration and helps with tolerances from small Arduino-compatible boards and low-cost RF transmitter modules.

## What It Does

- Drives a simple active-high 433 MHz OOK/ASK transmitter module directly from a GPIO pin.
- Replays the captured charge port opener bitstream.
- Provides a pulse-length sweep from 390 us to 410 us in 5 us steps.
- Includes blocking and non-blocking sketch variants.
- Keeps the original scope, SDR and conversion samples for reference.

The charge port only opens when the car allows it, for example when it is unlocked or the owner/key is nearby. This is not a vehicle unlock bypass.

## Hardware

Tested with Arduino-compatible boards and cheap 433 MHz transmitter modules. The project should work with most Arduino boards, ATtiny/Digispark-style boards and ESP32 boards supported by the Arduino IDE.

Known working transmitter example:

- H34S-433M 433 MHz OOK/ASK transmitter module

Typical wiring:

| RF module | Arduino-compatible board |
| --- | --- |
| DATA | `TX_PIN`, default `2` |
| VCC | Matching module supply voltage |
| GND | GND |

Some sketches also define `PUSH_BUTTON_PIN` and `BUTTON_GND_PIN`. On a Digispark, this lets you wire a small momentary button without needing a separate ground pin.

## Sketches

The active sketches are in `Tesla_Chargeportduino/`:

| Sketch | Use case |
| --- | --- |
| `Tesla_Chargeportduino-RF_sweep-blocking` | Recommended starting point. Simple pulse sweep, blocks while sending. |
| `Tesla_Chargeportduino-RF_sweep-only_TX_blocking` | Blocking RF bursts, non-blocking delay between sweep steps. |
| `Tesla_Chargeportduino-RF_sweep-nonblocking` | Non-blocking RF bit timing and non-blocking delay between sweep steps. |
| `Tesla_Chargeportduino-simple_transmission-blocking` | Fixed 400 us timing, useful after manual calibration. |

Historical first experiments are kept in `Tesla_Chargeportduino/old/`.

## Recommended Start

1. Open `Tesla_Chargeportduino/Tesla_Chargeportduino-RF_sweep-blocking/Tesla_Chargeportduino-RF_sweep-blocking.ino`.
2. Wire the transmitter DATA pin to `TX_PIN` (`2` by default).
3. Set your board in the Arduino IDE.
4. Upload the sketch.
5. Power-cycle or press the configured button to transmit one sweep sequence.

If your board has a slow bootloader, the transmission may start noticeably after power-up. For tiny USB boards such as Digispark, see `Tesla_Chargeportduino/Digispark no Startup delay/` for the old no-startup-delay bootloader notes.

## Timing Notes

The captured signal uses roughly 400 us slots. In early tests, a fixed timing value worked only after manual calibration. The sweep variant transmits the same bitstream multiple times with slightly different pulse lengths, which makes the result more tolerant of:

- board clock tolerance,
- Arduino core timing differences,
- RF module behavior,
- supply voltage and wiring differences.

The exact dominant source of error depends on the board/module combination. The sweep is intentionally simple and practical rather than a precise RF calibration routine.

## Samples

`Tesla_Chargeportduino/Datasamples/` contains the working notes and capture artifacts:

- `Bitstream & Array.txt` - bitstream and Arduino array representation.
- `Sample to Bitstream conversion.xls` - original spreadsheet used during conversion.
- `Scopesample/` - LabNation SmartScope logic analyzer captures and screenshots.
- `RF Sample/` - SDR-derived sample files.

These files are included so the project remains reproducible and inspectable rather than just a mystery array in a sketch.

## Safety And Legal

Use this only with your own car and follow local radio regulations. Do not misuse it.

## Alternatives And Further Reading

- [BetterTeslaChargeportOpener](https://github.com/stefan73/BetterTeslaChargeportOpener) - a good alternative implementation and useful information source. It recreates the Tesla charge port opener signal with a more structured preamble/Manchester/postamble approach.
- [rc-switch](https://github.com/sui77/rc-switch) - common Arduino/Raspberry Pi library for 315/433 MHz AM/OOK devices.
- [rtl_433](https://github.com/merbanan/rtl_433) - tool for receiving and analyzing ISM-band radio signals.

## License

MIT. See `LICENSE`.
