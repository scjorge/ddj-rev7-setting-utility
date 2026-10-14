# Raspberry Pi — DDJ-REV7 USB Auto-Config (planejado)

Espaço reservado para uma versão em Raspberry Pi (Linux). Um Raspberry Pi completo
(ex.: Zero 2 W, 3, 4, 5) é atraente porque as **portas USB-A já fornecem 5 V (VBUS)**,
então não precisa do truque do OTG com energia — você pluga o cabo de impressora da
controladora direto.

Abordagem pretendida (ainda não implementada aqui):
- Um script em Python usando **pyusb/libusb** que envia os mesmos control transfers
  documentados em [`../../docs/usb-command-map.md`](../../docs/usb-command-map.md),
  lendo os valores de [`../../config/config.ini`](../../config/config.ini).
- Uma regra `udev` para rodar automaticamente quando a DDJ-REV7 (VID `0x2B73`,
  PID `0x003B`) for conectada.

Contribuições são bem-vindas — veja **Contribuindo** no [README do projeto](../../README_PT.md).
