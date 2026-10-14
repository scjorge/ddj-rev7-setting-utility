idf_component_register(
    SRCS "main.c" "ini_parser.c" "led.c" "usb_sender.c" "msc_disk.c"
    INCLUDE_DIRS "."
    EMBED_TXTFILES "../storage/config.ini"
    REQUIRES usb fatfs wear_levelling vfs driver led_strip esp_tinyusb
)
