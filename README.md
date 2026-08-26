# Desk Robot

Desk Robot is an ESP32-based desk assistant that uses a TFT display, microphone, speaker, RTC, and buttons for voice activation, calls, and UI navigation.

---

## Pinout

This section documents the ESP32 wiring used by the Desk Robot project. All modules share a common 3.3V supply and GND unless otherwise noted.

---

### ILI9341 (TFT display)
| ESP32  | ILI9341 |
| ------ | ------- |
| GPIO23 | MOSI    |
| GPIO19 | MISO    |
| GPIO18 | SCK     |
| GPIO5  | CS      |
| GPIO2  | DC      |
| GPIO4  | RST     |
| 3V3    | VCC     |
| GND    | GND     |
| 3V3    | LED     |

### INMP441 (Microphone)
| ESP32  | INMP441    |
| ------ | ---------- |
| GPIO25 | WS / LRCLK |
| GPIO26 | SCK / BCLK |
| GPIO33 | SD         |
| 3V3    | VDD        |
| GND    | GND        |
| GND    | L/R        |

### MAX98357A (I2S Speaker / DAC)
| ESP32  | MAX98357A |
| ------ | --------- |
| GPIO27 | DIN       |
| GPIO14 | BCLK      |
| GPIO12 | LRC       |
| 3V3    | VIN       |
| GND    | GND       |

### Buzzer
| ESP32  | Buzzer |
| ------ | ------ |
| GPIO15 | +      |
| GND    | -      |

### DS3231 (RTC)
| ESP32  | DS3231 |
| ------ | ------ |
| GPIO21 | SDA    |
| GPIO22 | SCL    |
| 3V3    | VCC    |
| GND    | GND    |

### Buttons / Functions
| ESP32  | Function                      |
| ------ | ----------------------------- |
| GPIO13 | Answer Call                   |
| GPIO32 | Reject Call                   |
| GPIO35 | Wake Robot / Voice Activation |
| GPIO34 | Mode / Next Screen            |

---

## Consolidated GPIO Reference
| GPIO   | Purpose                  |
| ------ | ------------------------ |
| GPIO2  | TFT DC                   |
| GPIO4  | TFT Reset                |
| GPIO5  | TFT CS                   |
| GPIO12 | I2S LRCLK (Speaker)      |
| GPIO13 | Answer Button            |
| GPIO14 | I2S BCLK (Speaker)       |
| GPIO15 | Buzzer                   |
| GPIO18 | TFT SCK                  |
| GPIO19 | TFT MISO                 |
| GPIO21 | I2C SDA (RTC)            |
| GPIO22 | I2C SCL (RTC)            |
| GPIO23 | TFT MOSI                 |
| GPIO25 | I2S WS (Microphone)      |
| GPIO26 | I2S BCLK (Microphone)    |
| GPIO27 | I2S DIN (Speaker)        |
| GPIO32 | Reject Button            |
| GPIO33 | I2S SD (Microphone)      |
| GPIO34 | Mode Button (Input Only) |
| GPIO35 | Wake Button (Input Only) |

