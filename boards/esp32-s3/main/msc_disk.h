#pragma once
#include <stdbool.h>
// Inicializa o backend de armazenamento (particao FAT via wear-levelling + TinyUSB MSC).
bool storage_init(void);
// Monta a FAT para acesso LOCAL do ESP (para ler/gravar config.ini). Garante o
// config.ini padrao. Use antes de ler o arquivo no modo HOST.
bool storage_fs_mount_local(const char *mount_point);
// Solta a FAT do lado do ESP (libera para o PC). Chame antes de expor por USB.
void storage_fs_unmount_local(void);
// Expoe a particao como pendrive USB (instala o driver TinyUSB).
void msc_usb_start(void);
