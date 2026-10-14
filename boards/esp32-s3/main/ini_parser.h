#pragma once
#include <stdbool.h>
// Lê config.ini e, para a seção dada, devolve enabled e o número da opção (1-based).
// Retorna false se a seção não existir. Parser minimalista, tolerante a espaços e ";" comentários.
bool ini_get_setting(const char *path, const char *section, bool *enabled, int *option);
