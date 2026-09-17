# 🤖 Desk Robot

An ESP32-based smart desk robot designed to provide a compact desktop assistant with:

- Real Bluetooth Classic phone-call functionality
- Bluetooth Hands-Free Profile (HFP)
- OLED graphical user interface
- Animated robot face
- Real-time clock
- Physical call-control buttons
- Digital microphone
- Digital speaker
- Todo/reminder functionality
- Buzzer notifications
- Battery-powered operation
- Future voice-command interaction

The project is developed using **ESP-IDF** and **VS Code** for an ESP32-WROOM-32.

---

# 1. Project Overview

The Desk Robot is a small approximately **10 cm × 10 cm × 10 cm** desktop robot.

The main idea is to combine a small embedded system with:

```text
                    ┌─────────────────────┐
                    │      SMARTPHONE     │
                    │                     │
                    │  Bluetooth Classic  │
                    └──────────┬──────────┘
                               │
                               │ HFP
                               │
                               ▼
                    ┌─────────────────────┐
                    │      ESP32-WROOM-32 │
                    │                     │
                    │   HFP Hands-Free    │
                    │       Client        │
                    └──────┬──────┬───────┘
                           │      │
                    ┌──────┘      └──────┐
                    ▼                     ▼
              ┌──────────┐         ┌──────────┐
              │ INMP441  │         │MAX98357A │
              │   MIC    │         │  AUDIO   │
              └──────────┘         └────┬─────┘
                                        │
                                     Speaker

                    ┌─────────────────────┐
                    │      SSD1306 OLED   │
                    │       128 × 64      │
                    └─────────────────────┘

                    ┌─────────────────────┐
                    │       DS3231 RTC    │
                    └─────────────────────┘

                    ┌─────────────────────┐
                    │ Call / Control      │
                    │ Buttons + Buzzer    │
                    └─────────────────────┘
```

---

# 2. Main Features

## 2.1 Bluetooth Phone Calls

The robot can connect to a smartphone using:

**Bluetooth Classic Hands-Free Profile (HFP).**

The architecture is:

```text
Smartphone
     │
     │ Bluetooth Classic
     │
     ▼
Audio Gateway (AG)
     │
     │ HFP
     ▼
ESP32
Hands-Free (HF)
```

The smartphone acts as the:

```text
Audio Gateway (AG)
```

The ESP32 acts as the:

```text
Hands-Free Unit (HF)
```

The ESP32 can:

- Discover the phone
- Establish the HFP connection
- Establish the Service Level Connection (SLC)
- Detect incoming calls
- Detect active calls
- Answer calls
- Reject calls
- End calls
- Receive call state information
- Receive HFP audio
- Transmit microphone audio to the phone

---

# 3. Bluetooth HFP Architecture

HFP contains two important communication paths.

```text
             Bluetooth HFP
                  │
        ┌─────────┴─────────┐
        │                   │
        ▼                   ▼
   Control Channel      Audio Channel
        │                   │
        ▼                   ▼
     RFCOMM             SCO / eSCO
        │                   │
        ▼                   ▼
   AT Commands          Voice Audio
```

## 3.1 Control Channel

The control side is used for telephone functions such as:

- Call status
- Call setup
- Answer
- Reject
- Call termination
- Network status
- Signal strength
- Battery level
- Caller information

HFP uses an AT-command-based control protocol over the Bluetooth control connection.

---

# 4. HFP Audio

Voice audio is carried through the Bluetooth SCO/eSCO audio connection.

The current implementation uses the HCI audio data path.

The application-level HFP audio interface is:

```text
16-bit
8 kHz
Mono
PCM
```

The audio flow is:

```text
                  INCOMING AUDIO

Smartphone
    │
    │ Bluetooth HFP
    ▼
 SCO / eSCO
    │
    ▼
ESP-IDF HFP
    │
    ▼
16-bit / 8 kHz / Mono PCM
    │
    ▼
Audio Conversion
    │
    ▼
I²S
    │
    ▼
MAX98357A
    │
    ▼
Speaker
```

For microphone audio:

```text
INMP441
    │
    ▼
I²S
    │
    ▼
Audio Conversion
    │
    ▼
16-bit / 8 kHz / Mono PCM
    │
    ▼
ESP-IDF HFP
    │
    ▼
SCO / eSCO
    │
    ▼
Smartphone
```

---

# 5. CVSD and mSBC

HFP can use different speech codecs.

The current implementation is based around narrow-band HFP audio.

## CVSD

CVSD is the traditional narrow-band speech codec used by HFP.

Typical speech bandwidth:

```text
8 kHz sampling
```

The current application audio interface therefore uses:

```text
16-bit PCM
8 kHz
Mono
```

## mSBC

mSBC is associated with HFP Wideband Speech (WBS).

It provides approximately:

```text
16 kHz speech sampling
```

and provides higher speech bandwidth than narrow-band CVSD.

The current Desk Robot audio bridge is designed around the **8 kHz HFP PCM interface**.

---

# 6. I²S Audio Architecture

The INMP441 requires an appropriate I²S clock relationship.

The current implementation uses:

```text
Sample Rate = 8 kHz
Bits = 32
Channels = Stereo
```

This allows the I²S clocking required by the INMP441 while the application-level HFP data remains:

```text
16-bit
8 kHz
Mono
```

The software performs the conversion between the HFP audio format and the I²S hardware format.

```text
HFP:

16-bit / 8 kHz / Mono
          │
          ▼
     Audio Bridge
          │
          ▼
I²S: 32-bit / 8 kHz / Stereo
```

For the INMP441, the 32-bit stereo slot configuration produces:

```text
BCLK = 8,000 × 32 × 2
     = 512,000 Hz
```

Therefore:

```text
BCLK ≈ 512 kHz
```

---

# 7. Display

The original project used an ILI9341 TFT during the earlier development stage.

The current prototype uses:

```text
SSD1306
128 × 64
I²C OLED
Address: 0x3C
```

The OLED is used for:

- Time
- Date
- Animated robot face
- Incoming call notification
- Active call notification
- Call-ended status
- Todo/reminder information
- Other robot UI information

---

# 8. Normal Display Behaviour

The robot normally cycles through its user interface.

Current sequence:

```text
Happy Face
    │
    │ 5 seconds
    ▼
Date / Time
    │
    │ 5 seconds
    ▼
Normal Animated Face
    │
    │ 6 seconds
    ▼
Happy Face
    │
    └─────────────── repeat
```

Therefore:

```text
Face       = 5 seconds
Clock      = 5 seconds
Face       = 6 seconds
```

The face system supports several expressions:

```text
FACE_NORMAL
FACE_HAPPY
FACE_SAD
FACE_SURPRISED
FACE_SLEEP
FACE_TALKING
```

The face can also:

- Blink
- Move/animate the eyes
- Animate the mouth
- Change expressions

---

# 9. Call UI Behaviour

Incoming calls have priority over the normal display cycle.

For example:

```text
Normal UI
   │
   │ Incoming call
   ▼
Incoming Call Screen
   │
   │ Answer
   ▼
Active Call Screen
   │
   │ Call ends
   ▼
Normal UI resumes
```

The intended behaviour is:

### Incoming Call

The OLED displays an incoming-call state and the robot can notify the user using the buzzer.

### Answered Call

When the call is answered:

```text
CALL = IN_PROGRESS
```

the UI changes to the active-call display.

### Call Ended

When:

```text
CALL = NO_ACTIVE_CALL
```

the call UI is terminated and the normal display cycle resumes.

---

# 10. Important HFP Call-State Handling

HFP provides separate indicators for:

```text
CALL
CALL_SETUP
```

These two states must not be treated as the same thing.

For example, after answering an incoming call:

```text
CALL_SETUP = IDLE
```

may occur while:

```text
CALL = IN_PROGRESS
```

Therefore:

```text
CALL_SETUP = IDLE
```

does **not** necessarily mean that the phone call has ended.

The Desk Robot therefore uses the actual:

```text
CALL
```

indicator to determine whether a call is active.

Conceptually:

```text
CALL_SETUP = INCOMING
        │
        ▼
 Incoming Call
        │
        │ Answer
        ▼
CALL = IN_PROGRESS
        │
        ▼
   Active Call
        │
        │ Phone hangs up / End
        ▼
CALL = NO ACTIVE CALL
        │
        ▼
 Normal UI
```

This prevents the active-call screen from disappearing immediately after answering.

---

# 11. Physical Call Buttons

The current call-control buttons are:

| GPIO | Function |
|------|----------|
| GPIO32 | Answer Call |
| GPIO13 | End / Reject Call |

The buttons are active LOW.

Recommended wiring:

```text
GPIO32 ───── Button ───── GND
```

and:

```text
GPIO13 ───── Button ───── GND
```

The ESP32 input uses a pull-up.

Therefore:

```text
Button released = HIGH
Button pressed  = LOW
```

The software includes button debouncing.

---

# 12. Current GPIO Pinout

## SSD1306 OLED

| ESP32 | OLED |
|------|------|
| GPIO21 | SDA |
| GPIO22 | SCL |
| 3.3V | VCC |
| GND | GND |

OLED address:

```text
0x3C
```

---

## INMP441 Microphone

| ESP32 | INMP441 |
|------|---------|
| GPIO25 | BCLK / SCK |
| GPIO26 | WS / LRCLK |
| GPIO33 | SD |
| 3.3V | VDD |
| GND | GND |
| GND | L/R |

The L/R pin is connected to GND so the microphone operates on the selected channel.

---

## MAX98357A

The current audio architecture shares the I²S clock signals.

| ESP32 | MAX98357A |
|------|-----------|
| GPIO25 | BCLK |
| GPIO26 | LRC / LRCLK |
| GPIO27 | DIN |
| 5V | VIN |
| GND | GND |

The speaker connects to:

```text
MAX98357A OUT+
MAX98357A OUT-
```

The speaker should not be connected between OUT+ and GND.

---

## Call Buttons

| ESP32 | Function |
|------|----------|
| GPIO32 | Answer |
| GPIO13 | End / Reject |

Both buttons are active LOW.

---

## DS3231 RTC

| ESP32 | DS3231 |
|------|--------|
| GPIO21 | SDA |
| GPIO22 | SCL |
| 3.3V | VCC |
| GND | GND |

RTC I²C address:

```text
0x68
```

---

# 13. Consolidated GPIO Reference

| GPIO | Purpose |
|------|---------|
| GPIO13 | End / Reject Call |
| GPIO21 | I²C SDA |
| GPIO22 | I²C SCL |
| GPIO25 | I²S BCLK |
| GPIO26 | I²S WS / LRCLK |
| GPIO27 | I²S Speaker DIN |
| GPIO32 | Answer Call |
| GPIO33 | INMP441 Microphone Data |

---

# 14. GPIO Conflicts / Reserved Pins

GPIO26 is currently used as:

```text
I²S WS / LRCLK
```

Therefore, it must **not simultaneously be used as the buzzer GPIO**.

An older version of the project used:

```text
GPIO26 = Buzzer
```

but this conflicts with the current I²S configuration.

The current README therefore does **not** assign GPIO26 to the buzzer.

Before enabling the buzzer in the final hardware revision, a free GPIO must be selected and the software configuration must be updated accordingly.

---

# 15. DS3231 Real-Time Clock

The robot uses a DS3231 RTC to maintain the date and time.

The RTC communicates with the ESP32 using I²C.

```text
ESP32
 │
 ├── GPIO21 ─── SDA
 │
 └── GPIO22 ─── SCL
       │
       ▼
    DS3231
```

The RTC address is:

```text
0x68
```

The software reads:

- Seconds
- Minutes
- Hours
- Day
- Date
- Month
- Year

The RTC allows the robot to maintain time independently of the ESP32 software clock.

---

# 16. I²C Bus

Both the OLED and RTC use the same I²C bus.

```text
                 ┌── SSD1306
                 │     0x3C
                 │
ESP32 I²C Bus ───┤
                 │
                 └── DS3231
                       0x68
```

Current pins:

```text
SDA = GPIO21
SCL = GPIO22
```

Because the devices have different addresses, both can operate on the same I²C bus.

---

# 17. Audio Hardware

## Microphone

The project uses:

```text
INMP441
```

The INMP441 is a digital I²S MEMS microphone.

It provides digital audio directly to the ESP32.

Advantages for this project include:

- Digital output
- I²S interface
- No analog microphone amplifier required
- Suitable for embedded audio processing

---

## Speaker Amplifier

The project uses:

```text
MAX98357A
```

The MAX98357A is an I²S digital audio amplifier.

The ESP32 sends digital audio to:

```text
MAX98357A DIN
```

The amplifier drives the external speaker.

---

# 18. Audio Signal Path

## Receive Audio

When another person speaks to the phone:

```text
Phone
  │
  ▼
Bluetooth HFP
  │
  ▼
SCO / eSCO
  │
  ▼
ESP32 HFP
  │
  ▼
PCM Audio
  │
  ▼
I²S
  │
  ▼
MAX98357A
  │
  ▼
Speaker
```

---

## Transmit Audio

When the user speaks to the robot:

```text
User
  │
  ▼
INMP441
  │
  ▼
I²S
  │
  ▼
ESP32 Audio Bridge
  │
  ▼
PCM Audio
  │
  ▼
HFP
  │
  ▼
Bluetooth
  │
  ▼
Phone
```

---

# 19. Power System

The current power system uses two 3.7 V Li-ion cells in series.

```text
2 × 3.7 V Li-ion
      │
      ▼
7.4 V nominal
8.4 V fully charged
      │
      ▼
ON/OFF Switch
      │
      ▼
5 V Buck Converter
      │
      ├──────────────► ESP32 VIN / 5V
      │
      └──────────────► MAX98357A VIN
```

The buck converter reduces the battery voltage to approximately:

```text
5.0 V
```

The ESP32 receives the regulated 5 V through its VIN/5V input.

The MAX98357A can use the same 5 V rail.

---

# 20. Power Wiring

## Battery → Switch

```text
Battery +
    │
    ▼
ON/OFF switch
    │
    ▼
Buck Converter IN+
```

Battery negative:

```text
Battery -
    │
    ▼
Buck Converter IN-
```

## Buck Converter → ESP32

```text
Buck OUT+
    │
    ▼
ESP32 VIN / 5V

Buck OUT-
    │
    ▼
ESP32 GND
```

## Buck Converter → MAX98357A

```text
Buck OUT+
    │
    ▼
MAX98357A VIN

Buck OUT-
    │
    ▼
MAX98357A GND
```

The output of the buck converter should be verified with a multimeter before connecting the ESP32.

Target:

```text
≈ 5.0 V
```

---

# 21. Battery Configuration

The battery configuration is:

```text
2S Li-ion
```

which means:

```text
Cell 1 ── Cell 2
```

Nominal voltage:

```text
3.7 V × 2 = 7.4 V
```

Fully charged voltage:

```text
4.2 V × 2 = 8.4 V
```

For permanent battery-powered operation, the battery pack must have appropriate protection/balancing and must be charged using a charger designed for a 2S Li-ion pack.

---

# 22. Software Environment

Current development environment:

```text
Operating System:
Windows 11

IDE:
Visual Studio Code

Framework:
ESP-IDF

ESP-IDF Version:
v6.1-rc1

Microcontroller:
ESP32-WROOM-32
```

The ESP-IDF PowerShell environment is used for building, flashing and monitoring the project.

---

# 23. Useful ESP-IDF Commands

Build:

```powershell
idf.py build
```

Flash:

```powershell
idf.py -p COM10 flash
```

Monitor:

```powershell
idf.py -p COM10 monitor
```

Flash and monitor:

```powershell
idf.py -p COM10 flash monitor
```

Open configuration:

```powershell
idf.py menuconfig
```

---

# 24. HFP ESP-IDF Example

The HFP implementation was developed from the ESP-IDF Bluetooth Classic HFP Hands-Free example.

Relevant example:

```text
examples/bluetooth/bluedroid/classic_bt/hfp_hf
```

The project uses the ESP32 as the HFP client / Hands-Free device.

Important Bluetooth configuration includes:

```text
CONFIG_BT_HFP_ENABLE=y
CONFIG_BT_HFP_CLIENT_ENABLE=y
CONFIG_BT_HFP_AG_ENABLE=y
CONFIG_BT_HFP_AUDIO_DATA_PATH_HCI=y
```

The exact enabled options should always be checked in the project's current `sdkconfig`.

---

# 25. HFP Connection Process

The general connection process is:

```text
ESP32 starts Bluetooth
        │
        ▼
Device discovery
        │
        ▼
Phone discovered
        │
        ▼
HFP connection
        │
        ▼
Service Level Connection
        │
        ▼
HFP indicators configured
        │
        ▼
Phone call available
```

Once connected, the ESP32 receives HFP status information.

Examples include:

```text
CALL
CALL_SETUP
NETWORK
SIGNAL
BATTERY
```

---

# 26. Incoming Call Process

The current call process is approximately:

```text
Phone receives call
       │
       ▼
CALL_SETUP = INCOMING
       │
       ▼
ESP32 detects incoming call
       │
       ├── OLED → Incoming Call
       │
       └── Buzzer → Notification
       │
       ▼
User presses Answer
       │
       ▼
HFP Answer Command
       │
       ▼
CALL = IN_PROGRESS
       │
       ▼
Active Call UI
       │
       ▼
Microphone + Speaker active
```

---

# 27. Reject / End Call Process

For an incoming call:

```text
Incoming Call
     │
     ▼
END / REJECT button
     │
     ▼
HFP Reject Command
     │
     ▼
Call terminated
     │
     ▼
Normal UI
```

During an active call:

```text
Active Call
     │
     ▼
END button
     │
     ▼
HFP End / Reject Command
     │
     ▼
Call terminated
     │
     ▼
Normal UI resumes
```

---

# 28. Call Button Software

The call-button module is responsible for:

- Reading physical buttons
- Debouncing button presses
- Detecting incoming calls
- Detecting active calls
- Calling the HFP answer function
- Calling the HFP reject/end function

The button state is maintained separately from the HFP event handling.

The interface contains functions such as:

```c
call_buttons_init();

call_buttons_set_state();

call_is_incoming();

call_is_active();

call_answer();

call_reject();
```

This keeps physical button handling separate from the Bluetooth implementation.

---

# 29. Project Software Architecture

The project is divided into modules.

A simplified architecture is:

```text
main.c
 │
 ├── Display
 │    └── SSD1306
 │
 ├── Face
 │    └── Animated robot face
 │
 ├── RTC
 │    └── DS3231
 │
 ├── Call Display
 │    └── Incoming / Active / Ended
 │
 ├── Call Buttons
 │    └── Answer / End
 │
 ├── HFP
 │    ├── Bluetooth connection
 │    ├── HFP events
 │    └── Call control
 │
 └── HFP Audio
      ├── RX PCM
      ├── TX PCM
      ├── I²S
      ├── INMP441
      └── MAX98357A
```

---

# 30. Important Source Files

The project contains modules such as:

```text
main/
│
├── main.c
│
├── bt_app_hf.c
├── bt_app_hf.h
│
├── hfp_audio.c
├── hfp_audio.h
│
├── call_buttons.c
├── call_buttons.h
│
├── call_display.c
├── call_display.h
│
├── display.c
├── display.h
│
├── face.c
├── face.h
│
├── rtc.c
├── rtc.h
│
├── i2c_bus.c
├── i2c_bus.h
│
└── config.h
```

The exact directory structure may vary as modules are reorganized.

---

# 31. Display Module

The display module provides low-level SSD1306 functions such as:

```c
display_clear();
display_update();
display_text();
display_center_text();
display_draw_hline();
display_draw_rect();
display_fill_rect();
```

The display uses a framebuffer:

```text
128 × 64 pixels
```

which corresponds to:

```text
128 × 64 / 8
= 1024 bytes
```

for a 1-bit monochrome framebuffer.

---

# 32. Face Module

The face module provides:

```c
face_init();
face_set();
face_update();
face_draw();
```

Supported expressions:

```c
FACE_NORMAL
FACE_HAPPY
FACE_SAD
FACE_SURPRISED
FACE_SLEEP
FACE_TALKING
```

The animation system can control:

- Eye blinking
- Eye movement
- Mouth animation
- Facial expression

---

# 33. RTC Module

The RTC module communicates with the DS3231 through the shared I²C bus.

Typical operations include:

```c
rtc_init();
rtc_read();
```

The RTC converts the DS3231's BCD register values into normal `struct tm` date/time values for use by the application.

---

# 34. Voice Commands

Voice command control is planned as a future feature.

The intended concept is:

```text
User speaks
     │
     ▼
INMP441
     │
     ▼
Audio processing
     │
     ▼
Voice command recognition
     │
     ▼
Command
     │
     ├── Answer
     ├── Reject
     ├── Hang up
     ├── Wake robot
     └── Other commands
```

Possible commands include:

```text
"Answer"
"Reject"
"Hang up"
```

Voice recognition is **not currently documented as a completed feature** until the recognition pipeline is fully implemented and tested.

The existing HFP audio path should be considered when implementing voice recognition because the INMP441 is already part of the call-audio system.

---

# 35. Todo and Reminder System

A planned/partially implemented feature is a todo and reminder system.

The intended behaviour is:

```text
Todo item
    │
    ▼
Stored task time
    │
    ├── Reminder before task
    │
    └── Alarm at task time
```

The OLED can display the task and the buzzer can provide an audible notification.

The exact final storage and scheduling implementation is still under development.

---

# 36. System Priority

The UI is designed so that important events can interrupt the normal display cycle.

Conceptually:

```text
                 ┌──────────────────┐
                 │   Normal UI      │
                 │ Face / Clock     │
                 └────────┬─────────┘
                          │
                 Event detected
                          │
              ┌───────────┴───────────┐
              │                       │
              ▼                       ▼
        Incoming Call             Todo Alarm
              │                       │
              ▼                       ▼
        Call UI / Audio          Reminder UI
              │
              ▼
         Call Finished
              │
              ▼
        Normal UI resumes
```

Calls have priority because they require real-time user interaction.

---

# 37. Current Project Status

| Feature | Status |
|--------|--------|
| ESP32-WROOM-32 | ✅ Working |
| ESP-IDF environment | ✅ Working |
| Bluetooth Classic | ✅ Working |
| HFP discovery | ✅ Tested |
| HFP SLC | ✅ Tested |
| Incoming call detection | ✅ Working |
| Call answering | ✅ Working |
| Call rejection/end | ✅ Working |
| Active call detection | ✅ Working |
| HFP SCO/eSCO audio | ✅ Implemented |
| INMP441 | ✅ Integrated |
| MAX98357A | ✅ Integrated |
| OLED SSD1306 | ✅ Working |
| DS3231 RTC | ✅ Detected / integrated |
| Animated face | ✅ Implemented |
| Normal UI cycle | ✅ Implemented |
| Physical call buttons | 🔧 Hardware/testing |
| Buzzer | ⚠️ GPIO assignment needs resolution |
| Todo/reminders | 🔧 Under development |
| Voice commands | 🔧 Planned |
| Full final enclosure | 🔧 Under development |

---

# 38. Known Hardware Considerations

## GPIO26 Conflict

An older configuration assigned GPIO26 to the buzzer.

The current I²S system uses:

```text
GPIO26 = I²S WS / LRCLK
```

Therefore the buzzer must be moved to another available GPIO before the final integrated hardware is completed.

---

## Button Wiring

The ESP32 GPIO inputs used for buttons must have a defined idle state.

The intended configuration is:

```text
GPIO
 │
 ├── Internal pull-up
 │
 Button
 │
GND
```

Therefore:

```text
Released → HIGH
Pressed  → LOW
```

---

## Common Ground

All modules connected to the same power system must share a common ground.

For example:

```text
ESP32 GND
   │
   ├── OLED GND
   ├── RTC GND
   ├── INMP441 GND
   ├── MAX98357A GND
   └── Power supply GND
```

---

# 39. Development Workflow

Typical development cycle:

```text
1. Modify source code
       │
       ▼
2. Build
       │
       ▼
3. Flash ESP32
       │
       ▼
4. Open serial monitor
       │
       ▼
5. Test hardware
       │
       ▼
6. Debug
       │
       ▼
7. Commit changes
       │
       ▼
8. Push to GitHub
```

---

# 40. GitHub Repository

Project repository:

```text
Desk_Robot
```

GitHub:

```text
https://github.com/Dineth-W/Desk_Robot.git
```

---

# 41. Git Commands

Check the current state:

```powershell
git status
```

Add all changes:

```powershell
git add .
```

Commit:

```powershell
git commit -m "Update Desk Robot"
```

Push:

```powershell
git push origin main
```

Typical complete workflow:

```powershell
git status
git add .
git commit -m "Update Desk Robot"
git push origin main
```

---

# 42. Design Goals

The final Desk Robot is intended to combine:

```text
                 ┌───────────────────┐
                 │    DESK ROBOT     │
                 └─────────┬─────────┘
                           │
       ┌───────────────────┼───────────────────┐
       │                   │                   │
       ▼                   ▼                   ▼
   Communication        Interaction          Time
       │                   │                   │
       ▼                   ▼                   ▼
      HFP              OLED / Face           RTC
       │                   │                   │
       ▼                   ▼                   ▼
   Phone Calls       Buttons / Voice       DS3231
       │
       ▼
  Audio System
       │
   ┌───┴────┐
   ▼        ▼
 INMP441  MAX98357A
   │        │
   ▼        ▼
  Mic     Speaker
```

The goal is to create a compact embedded desk assistant that combines communication, time management, notifications, audio interaction and a visual robot interface.

---

# 43. Future Development

Planned improvements include:

- Voice activation
- Offline voice-command recognition
- Voice commands for answering/rejecting calls
- Todo-list management
- Scheduled reminders
- Alarm notifications
- Improved caller ID display
- Improved call UI
- More expressive face animations
- Improved audio processing
- Final PCB / wiring optimization
- Final enclosure
- Battery-management integration
- Improved power management

---

# 44. Project Summary

The current Desk Robot is based on:

```text
MCU
└── ESP32-WROOM-32

Communication
└── Bluetooth Classic HFP

Display
└── SSD1306 128×64 OLED

RTC
└── DS3231

Microphone
└── INMP441 I²S MEMS microphone

Audio amplifier
└── MAX98357A I²S amplifier

Call control
├── Answer button
└── End / Reject button

Power
└── 2S Li-ion → 5V buck converter
```

The most important implemented feature is the **real Bluetooth Classic HFP phone-call system**, allowing the ESP32 robot to function as a Bluetooth hands-free device for a smartphone.

---

