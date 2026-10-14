#pragma once
#include <stdbool.h>
// Entra em modo USB HOST, espera a DDJ-REV7 (VID/PID), aplica config.ini.
// Para cada ajuste: envia o control transfer (SET) e confirma lendo de volta (GET).
// Retorna o nº de ajustes aplicados com sucesso; <0 em erro de host/enum.
int rev7_apply_from_ini(const char *ini_path);
