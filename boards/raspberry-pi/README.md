# Raspberry Pi — DDJ-REV7 USB Auto-Config (planned)

Placeholder for a Raspberry Pi (Linux) version. A full Raspberry Pi (e.g. Zero 2 W,
3, 4, 5) is attractive because its **USB-A ports already supply 5 V (VBUS)**, so no
powered-OTG trick is needed — you plug the controller's printer cable directly.

Intended approach (not yet implemented here):
- A small Python script using **pyusb/libusb** that sends the same control transfers
  documented in [`../../docs/usb-command-map.md`](../../docs/usb-command-map.md),
  reading values from [`../../config/config.ini`](../../config/config.ini).
- A `udev` rule to run it automatically when the DDJ-REV7 (VID `0x2B73`,
  PID `0x003B`) is connected.

Contributions welcome — see **Contributing** in the [project README](../../README.md).
