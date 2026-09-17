# Desk Robot

An ESP32-based desk robot with an OLED display, real-time clock, Bluetooth Hands-Free calling, microphone, speaker, buttons, and battery power.

---

## Components

| Component | Purpose |
|---|---|
| ESP32 | Main microcontroller and Bluetooth HFP device |
| SSD1306 128×64 OLED | User interface and status display |
| DS3231 RTC | Real-time clock |
| INMP441 | Digital I²S microphone |
| MAX98357A | I²S audio amplifier |
| Speaker | Call audio output |
| Push buttons | Answer and end/reject calls |
| 2 × 3.7 V Li-ion cells | Battery supply |
| 5 V Buck Converter | Regulates battery voltage to 5 V |
| Buzzer | Audible notifications |

---

## Pin Configuration

### OLED – SSD1306

| ESP32 | OLED |
|---|---|
| GPIO21 | SDA |
| GPIO22 | SCL |
| 3.3 V | VCC |
| GND | GND |

I²C address: `0x3C`

### RTC – DS3231

| ESP32 | DS3231 |
|---|---|
| GPIO21 | SDA |
| GPIO22 | SCL |
| 3.3 V | VCC |
| GND | GND |

I²C address: `0x68`

The OLED and RTC share the same I²C bus.

### INMP441 Microphone

| ESP32 | INMP441 |
|---|---|
| GPIO25 | BCLK / SCK |
| GPIO26 | WS / LRCLK |
| GPIO33 | SD |
| 3.3 V | VDD |
| GND | GND |
| GND | L/R |

### MAX98357A

| ESP32 | MAX98357A |
|---|---|
| GPIO25 | BCLK |
| GPIO26 | LRC / LRCLK |
| GPIO27 | DIN |
| 5 V | VIN |
| GND | GND |

The I²S clock lines are shared between the microphone and speaker.

### Call Buttons

| ESP32 | Function |
|---|---|
| GPIO32 | Answer |
| GPIO13 | End / Reject |

Buttons are configured as active LOW:

```text
GPIO ─── Button ─── GND
