# HFP AUDIO BRIDGE PATCH

## Root cause found in the uploaded project

The existing `bt_app_hf.c` uses the ESP-IDF example's single `m_rb` ring buffer for BOTH HFP directions:

- HFP incoming callback writes Bluetooth RX audio into `m_rb`.
- HFP outgoing callback reads from the SAME `m_rb` and sends it back to Bluetooth.

That is the official example's loopback behavior. It is intentionally an echo test, not a speaker/microphone implementation.

Therefore:
- MAX98357A was never written by the application.
- INMP441 was never read by the HFP outgoing callback.
- The `HFP AUDIO RX/TX` counters proved only that the Bluetooth transport was active.

## Patch

Added `main/hfp_audio.c/.h`:
- Separate HFP->speaker and microphone->HFP ring buffers.
- I2S full-duplex on I2S0.
- 8 kHz sample rate for narrow-band HFP/CVSD.
- 32-bit stereo I2S slots to satisfy INMP441's 64 SCK cycles/frame.
- GPIO25 BCLK
- GPIO26 WS
- GPIO27 I2S OUT to MAX98357A
- GPIO33 I2S IN from INMP441
- HFP RX 16-bit mono PCM -> 32-bit stereo I2S for MAX98357A.
- INMP441 left-channel 32-bit I2S -> 16-bit mono PCM for HFP TX.
- HFP outgoing callback remains non-blocking.

Modified:
- `main/bt_app_hf.c`
- `main/main.c`
- `main/CMakeLists.txt`
- `main/gpio_pcm_config.h`

AEC GPIO setup was disabled because the existing test code drove GPIO21/22 as outputs, which conflicts with the project's OLED I2C SDA/SCL pins.

## Important verified facts

- `CONFIG_BT_HFP_AUDIO_DATA_PATH_HCI=y`
- `CONFIG_BT_HFP_WBS_ENABLE` is disabled.
- The runtime log reports `--audio state connected`.
- HFP packets are 120 bytes.
- 120 bytes = 60 samples at 16-bit PCM.
- 60 samples at 8 kHz = 7.5 ms, matching the observed HFP packet cadence.
- ESP-IDF documentation describes the HCI callback as the payload of the HCI synchronous audio packet.
- ESP-IDF's HFP example README states that with vHCI/HCI datapath and WBS disabled, CVSD is used and the hardware is responsible for the codec.
- The uploaded project's current loopback implementation is therefore the reason audio never reached the hardware.

## Do not change

Keep HFP call-control logic intact:
- discovery
- connection
- SLC
- incoming call
- caller ID
- answer
- active call
- disconnect

## First test after applying the patch

1. Connect INMP441 and MAX98357A using the project GPIO map.
2. Build.
3. Flash.
4. Monitor.
5. Make a call.
6. Confirm:
   `HFP PCM audio ACTIVE`
7. Speak into the phone. Robot speaker should reproduce the caller audio.
8. Speak toward INMP441. The phone should receive the microphone audio.

If speaker works but microphone does not, the next investigation should be ONLY the INMP441/I2S RX format/level, not Bluetooth HFP control.
