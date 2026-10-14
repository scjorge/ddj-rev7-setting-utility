# DDJ-REV7 Setting Utility

For use with apps outside Windows/Mac there is no **DDJ REV 7 Setting Utility**,
which makes it impossible to configure important things like the audio routing of
output channels 5/6 — the routing that lets software such as **DJay Pro** record
your mix.

The REV 7 itself wipes all memory when powered off and loses every saved setting.
On top of that, iOS blocks apps from sending the USB signals this needs. So this
project is a firmware for an **external microcomputer**: in practice you connect the
DDJ-REV7 to the board first, wait for it to apply the settings, then connect it
normally to iOS.

> **Note 1:** Only the effect options are not mappable.
>
> **Note 2:** The project was built with the **ESP32-S3-N8R2** board. If you want to
> contribute usage notes for other models, feel free to open a PR.

🇧🇷 [Versão em português](README_PT.md)

---

## How it was made

The project was made by monitoring the official utility's USB activity and
replaying each event (control transfer) to the REV 7. The full command map is in
[`docs/usb-command-map.md`](docs/usb-command-map.md).

## How it works

1. Connect the DDJ-REV7 **to the board** (microcomputer) and power it.
2. The board detects the controller and sends the `config.ini` settings over USB —
   the same ones the official utility would send.
3. Then you move the cable to iOS/PC and use it normally (settings persist until the
   controller is powered off).

`config.ini` is editable: the board also shows up as a USB flash drive.

## Supported boards

| Board | Status | Guide |
|---|---|---|
| **ESP32-S3** (tested on ESP32-S3-N8R2) | ✅ working | [boards/esp32-s3](boards/esp32-s3/README.md) |
| Raspberry Pi (Linux) | 🧩 planned / contributions | [boards/raspberry-pi](boards/raspberry-pi/README.md) |

> Want to port it to another board? See **Contributing** below.

## Repository layout

```
ddj-rev7-usb-config/
  README.md / README_PT.md     project overview (this file)
  docs/                        USB protocol (board-independent)
    usb-command-map.md           command map (human-readable)
    usb-commands.json            commands as JSON
  config/
    config.ini                   canonical reference configuration
  boards/
    esp32-s3/                    ESP-IDF firmware for ESP32-S3  (✅)
      README.md / README_PT.md
      main/ storage/ ...
    raspberry-pi/                placeholder for a Raspberry Pi version
      README.md / README_PT.md
```

## Documentation

- **Protocol / command map:** [`docs/usb-command-map.md`](docs/usb-command-map.md) and [`docs/usb-commands.json`](docs/usb-commands.json).
- **Reference configuration:** [`config/config.ini`](config/config.ini).
- **Per-board guide:** see your board's folder under `boards/`.

## Contributing

To add support for another board:
1. Create `boards/<your-board>/` with that platform's firmware/code.
2. Reuse the protocol in [`docs/`](docs/) and the configuration in [`config/`](config/).
3. Include a `README.md` and `README_PT.md` in the board folder (wiring, build, usage, LED/indicators).
4. Open a PR. The core (command map and `config.ini`) is the same for all boards;
   only each platform's USB-host layer changes.

## Disclaimer

This project only sends the documented USB configuration commands, like the official
app does — it does **not** modify the controller's firmware. Validate on your own
hardware. Use at your own risk.
