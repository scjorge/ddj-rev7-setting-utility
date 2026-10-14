# ESP32-S3 — DDJ-REV7 USB Auto-Config

Firmware for an **ESP32-S3** board (developed on **ESP32-S3-N8R2**). See the
[project overview](../../README.md) for what this does and why. This page is the
board-specific build, wiring and usage guide.

## LED indicators (on-board RGB, GPIO48)

| LED | State | Meaning |
|---|---|---|
| 🔵 Blue (dim) | idle / ready | powered; 3 s window to tap BOOT, or in edit mode |
| 🟠 Amber **blinking** | searching | host mode, looking for the DDJ-REV7 (~15 s) |
| 🟠 Amber **solid** | sending | controller found, applying `config.ini` |
| 🟢 Green | done | all enabled settings sent and confirmed |
| 🔴 Red | error | not a DDJ-REV7, transfer failure, or FAT mount failure |

Normal sequence when applying: **blue → amber blinking → amber solid → green**.

## Hardware / wiring (required)

The ESP32-S3's USB host port **does not output 5 V (VBUS)**, and the controller
will not enumerate without it. Use a **powered USB-OTG adapter/cable** (with an
extra power input). Confirmed working setup:

```
powerbank/charger (5V)
      │
      ▼ (power input of the OTG adapter)
ESP32-S3 (native USB) ─► powered OTG adapter ─► USB-A → USB-B "printer" cable ─► DDJ-REV7 (on)
```

- Power the board from the OTG adapter's power input (or a separate 5 V source).
- The controller connects to the **board**, never to the computer.
- The DDJ-REV7 is self-powered — keep it switched on.

## Build & flash (macOS, ESP-IDF v5.5)

Requires **ESP-IDF v5.5** and **Python 3.12** (the macOS system Python 3.9 breaks
the IDF dependency checker).

### One-time setup
```bash
brew install python@3.12
mkdir -p ~/esp && cd ~/esp
git clone -b v5.5 --recursive https://github.com/espressif/esp-idf.git
cd ~/esp/esp-idf
unset IDF_PYTHON_ENV_PATH VIRTUAL_ENV
/opt/homebrew/bin/python3.12 tools/idf_tools.py install
/opt/homebrew/bin/python3.12 tools/idf_tools.py install-python-env
/opt/homebrew/bin/python3.12 tools/idf_tools.py install cmake ninja
```

### Every new terminal
```bash
export PATH="/opt/homebrew/bin:$PATH"
export IDF_PYTHON_ENV_PATH="$HOME/.espressif/python_env/idf5.5_py3.12_env"
export SDKROOT=/Library/Developer/CommandLineTools/SDKs/MacOSX26.5.sdk   # any non-27.0 SDK
cd ~/esp/esp-idf && . ./export.sh
```

### Build / flash / monitor
```bash
cp -R "/path/to/boards/esp32-s3" ~/esp/rev7-esp32-firmware
cd ~/esp/rev7-esp32-firmware
idf.py set-target esp32s3
idf.py build
idf.py -p PORT flash monitor
```
The component manager downloads `esp_tinyusb` and `led_strip` on the first build.

## Usage

### Apply settings to the controller
1. Turn the DDJ-REV7 on.
2. Connect the board to the controller through the powered OTG adapter.
   LED: amber blinking → amber solid → green.
3. Move the cable to the iPad/PC **without powering the controller off**
   (settings are volatile and per USB port).

### Edit config.ini
1. Connect the board to a computer. When the LED is **blue**, **tap BOOT** once
   (within ~3 s). Do not hold BOOT during reset (that enters the ESP download mode).
2. A USB drive `DDJ REV 7` appears. Edit `config.ini`
   (`enabled = yes/no`, `option = N`), save, eject.

The default `config.ini` is embedded from [`storage/config.ini`](storage/config.ini);
the canonical copy lives in [`config/config.ini`](../../config/config.ini).

## Layout

```
boards/esp32-s3/
  main/
    main.c             boot flow and mode selection
    usb_sender.c       HOST mode: sends + confirms the vendor control transfers
    ini_parser.c       config.ini reader
    msc_disk.c         EDIT mode: USB mass-storage over the FAT partition
    led.c              status LED
    rev7_map.h         command table (generated from the USB capture)
    idf_component.yml  managed deps (esp_tinyusb, led_strip)
    CMakeLists.txt
  storage/config.ini   default config embedded on first boot
  partitions.csv       FAT "storage" partition for config.ini
  sdkconfig.defaults   S3, 8MB flash, PSRAM, TinyUSB MSC, FATFS label, hub support
```

## Troubleshooting

| Symptom | Cause | Fix |
|---|---|---|
| LED stays amber-blinking, never solid | controller not seen = no VBUS | use a **powered** OTG adapter; controller on the **board**, not the PC |
| Red LED at start | FAT mount failed | `idf.py erase-flash` then flash again |
| Serial port disappears when amber blinks | single USB port: host mode takes it over | normal; a board with a separate UART bridge port shows host logs |
| `requirements not satisfied` (IDF checker) | old IDF + new Python | use ESP-IDF **5.5** + Python **3.12** |
| `ld: tapi error ... MacOSX27.0.sdk` | broken macOS SDK | `export SDKROOT=.../MacOSX26.5.sdk` |
| `CONFIG_TINYUSB_MSC_BUFSIZE ... must be >= WL_SECTOR_SIZE` | buffer < sector | `CONFIG_TINYUSB_MSC_BUFSIZE=4096` (already set) |
