# HFP Audio Bridge — Build Fix v2

Fixed the two compile errors reported by ESP-IDF v6.1-rc1:

1. `hfp_audio.c`
   - Added the missing `hfp_pcm[240]` receive buffer used by `speaker_task()`.

2. `bt_app_hf.c`
   - Removed the duplicated `#endif` that caused:
     `error: '#endif' without '#if'`.

No Bluetooth/HFP configuration was changed.

The intended audio path remains:

- HFP RX -> speaker queue -> I2S TX -> MAX98357A
- INMP441 -> I2S RX -> microphone queue -> HFP TX

Current HFP configuration remains HCI audio path with WBS/mSBC disabled.
