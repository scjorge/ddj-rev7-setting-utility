// rev7_map.h - gerado automaticamente a partir da captura USB. Nao editar a mao.
// Cada ajuste: nome da secao [ini], wIndex, e a lista de opcoes (option N = indice N, base 1).
#pragma once
#include <stdint.h>

typedef struct { uint16_t wValue; const char *label; } rev7_option_t;
typedef struct { const char *section; uint16_t wIndex; const rev7_option_t *options; int n_options; } rev7_setting_t;

static const rev7_option_t OPT_routing_usb1_2[] = {
    { 0x010A, "MIX (REC OUT with MIC)" },
    { 0x0115, "MIX (REC OUT without MIC)" },
    { 0x0111, "Post CH1 Fader" },
    { 0x0107, "CROSSFADER A" },
    { 0x0108, "CROSSFADER B" },
    { 0x0109, "MIC" },
    { 0x010D, "AUX" },
    { 0x0103, "Control Tone" },
};
static const rev7_option_t OPT_routing_usb3_4[] = {
    { 0x020A, "MIX (REC OUT with MIC)" },
    { 0x0215, "MIX (REC OUT without MIC)" },
    { 0x0212, "Post CH2 Fader" },
    { 0x0207, "CROSSFADER A" },
    { 0x0208, "CROSSFADER B" },
    { 0x0209, "MIC" },
    { 0x020D, "AUX" },
    { 0x0203, "Control Tone" },
};
static const rev7_option_t OPT_routing_usb5_6[] = {
    { 0x0315, "MIX (REC OUT without MIC)" },
    { 0x0307, "CROSSFADER A" },
    { 0x0308, "CROSSFADER B" },
    { 0x0309, "MIC" },
    { 0x030D, "AUX" },
    { 0x0311, "Post CH1 Fader" },
    { 0x0312, "Post CH2 Fader" },
    { 0x030A, "MIX (REC OUT with MIC)" },
};
static const rev7_option_t OPT_output_level[] = {
    { 0x0000, "-19 dB" },
    { 0x0100, "-15 dB" },
    { 0x0200, "-10 dB" },
    { 0x0300, "-5 dB" },
};
static const rev7_option_t OPT_crossfader_fade[] = {
    { 0x0000, "SHARP" },
    { 0x0100, "NONE" },
    { 0x0200, "SMALL" },
    { 0x0300, "LARGE" },
};
static const rev7_option_t OPT_crossfader_cut_lag[] = {
    { 0x0100, "position 1" },
    { 0x0200, "position 2" },
    { 0x0300, "position 3" },
    { 0x0400, "position 4" },
    { 0x0500, "position 5" },
    { 0x0600, "position 6" },
    { 0x0700, "position 7" },
    { 0x0800, "position 8" },
    { 0x0900, "position 9" },
    { 0x0A00, "position 10" },
};
static const rev7_option_t OPT_loop_button_type[] = {
    { 0x0000, "type A" },
    { 0x0100, "type B" },
};
static const rev7_option_t OPT_pad_mode[] = {
    { 0x0000, "mode A" },
    { 0x0100, "mode B" },
};
static const rev7_option_t OPT_loop_button_length[] = {
    { 0x0700, "preset 7" },
    { 0x0800, "preset 8" },
    { 0x0A00, "preset 10" },
    { 0x0B00, "preset 11" },
    { 0x0C00, "preset 12" },
    { 0x0D00, "preset 13" },
    { 0x0E00, "preset 14" },
};
static const rev7_option_t OPT_time_mode[] = {
    { 0x0000, "Elapsed" },
    { 0x0100, "Remain" },
};
static const rev7_option_t OPT_hot_cue_countdown[] = {
    { 0x0000, "Off" },
    { 0x0100, "On" },
};
static const rev7_option_t OPT_rpm_button_mode[] = {
    { 0x0000, "Instant" },
    { 0x0100, "Long Press" },
};
static const rev7_option_t OPT_slow_speed_stabilizer[] = {
    { 0x0000, "Off" },
    { 0x0100, "On" },
};

static const rev7_setting_t REV7_SETTINGS[] = {
    { "routing_usb1_2", 0xC002, OPT_routing_usb1_2, 8 },
    { "routing_usb3_4", 0xC002, OPT_routing_usb3_4, 8 },
    { "routing_usb5_6", 0xC002, OPT_routing_usb5_6, 8 },
    { "output_level", 0xC003, OPT_output_level, 4 },
    { "crossfader_fade", 0xC215, OPT_crossfader_fade, 4 },
    { "crossfader_cut_lag", 0xC211, OPT_crossfader_cut_lag, 10 },
    { "loop_button_type", 0xC212, OPT_loop_button_type, 2 },
    { "pad_mode", 0xC214, OPT_pad_mode, 2 },
    { "loop_button_length", 0xC223, OPT_loop_button_length, 7 },
    { "time_mode", 0xC226, OPT_time_mode, 2 },
    { "hot_cue_countdown", 0xC227, OPT_hot_cue_countdown, 2 },
    { "rpm_button_mode", 0xC22D, OPT_rpm_button_mode, 2 },
    { "slow_speed_stabilizer", 0xC22E, OPT_slow_speed_stabilizer, 2 },
};
static const int REV7_N_SETTINGS = 13;

#define REV7_VID 0x2B73
#define REV7_PID 0x003B
#define REV7_BREQUEST_SET 0x03
#define REV7_BREQUEST_GET 0x00
#define REV7_BM_SET 0x40  // OUT | Vendor | Device
#define REV7_BM_GET 0xC0  // IN  | Vendor | Device
