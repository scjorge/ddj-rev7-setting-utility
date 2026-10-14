# ESP32-S3 — DDJ-REV7 USB Auto-Config

Firmware para uma placa **ESP32-S3** (desenvolvido na **ESP32-S3-N8R2**). Veja a
[visão geral do projeto](../../README_PT.md) para o que ele faz e por quê. Esta
página é o guia específico desta placa: compilação, ligação e uso.

## Indicadores de LED (RGB on-board, GPIO48)

| LED | Estado | Significado |
|---|---|---|
| 🔵 Azul (fraco) | pronto / aguardando | ligado; janela de 3 s para tocar BOOT, ou em modo edição |
| 🟠 Âmbar **piscando** | procurando | modo host, buscando a DDJ-REV7 (~15 s) |
| 🟠 Âmbar **fixo** | enviando | controladora encontrada, aplicando o `config.ini` |
| 🟢 Verde | concluído | todos os ajustes habilitados enviados e confirmados |
| 🔴 Vermelho | erro | não é DDJ-REV7, falha de envio, ou falha ao montar o FAT |

Sequência normal ao aplicar: **azul → âmbar piscando → âmbar fixo → verde**.

## Hardware / ligação (obrigatório)

A porta USB host da ESP32-S3 **não fornece 5 V (VBUS)**, e a controladora não
enumera sem isso. Use um **adaptador/cabo USB-OTG com alimentação** (com uma
entrada de energia extra). Montagem confirmada funcionando:

```
powerbank/carregador (5V)
      │
      ▼ (entrada de energia do adaptador OTG)
ESP32-S3 (USB nativa) ─► adaptador OTG com energia ─► cabo USB-A → USB-B ("impressora") ─► DDJ-REV7 (ligada)
```

- Alimente a placa pela entrada de energia do adaptador OTG (ou um 5 V separado).
- A controladora conecta na **placa**, nunca no computador.
- A DDJ-REV7 é autoalimentada — mantenha-a ligada.

## Compilar e gravar (macOS, ESP-IDF v5.5)

Requer **ESP-IDF v5.5** e **Python 3.12** (o Python 3.9 do sistema macOS quebra o
verificador de dependências do IDF).

### Preparação (uma vez)
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

### A cada terminal novo
```bash
export PATH="/opt/homebrew/bin:$PATH"
export IDF_PYTHON_ENV_PATH="$HOME/.espressif/python_env/idf5.5_py3.12_env"
export SDKROOT=/Library/Developer/CommandLineTools/SDKs/MacOSX26.5.sdk   # qualquer SDK que nao seja o 27.0
cd ~/esp/esp-idf && . ./export.sh
```

### Build / flash / monitor
```bash
cp -R "/caminho/para/boards/esp32-s3" ~/esp/rev7-esp32-firmware
cd ~/esp/rev7-esp32-firmware
idf.py set-target esp32s3
idf.py build
idf.py -p PORTA flash monitor
```
O component manager baixa o `esp_tinyusb` e o `led_strip` na primeira compilação.

## Uso

### Aplicar os ajustes na controladora
1. Ligue a DDJ-REV7.
2. Conecte a placa à controladora através do adaptador OTG com alimentação.
   LED: âmbar piscando → âmbar fixo → verde.
3. Passe o cabo para o iPad/PC **sem desligar a controladora**
   (os ajustes são voláteis e por porta USB).

### Editar o config.ini
1. Conecte a placa a um computador. Quando o LED ficar **azul**, **toque o BOOT**
   uma vez (em até ~3 s). Não segure o BOOT durante o reset (isso entra no modo de
   gravação do ESP).
2. Aparece um pendrive `DDJ REV 7`. Edite o `config.ini`
   (`enabled = yes/no`, `option = N`), salve e ejete.

O `config.ini` padrão é embutido de [`storage/config.ini`](storage/config.ini); a
cópia canônica fica em [`config/config.ini`](../../config/config.ini).

## Estrutura

```
boards/esp32-s3/
  main/
    main.c             fluxo de boot e seleção de modo
    usb_sender.c       modo HOST: envia e confirma os control transfers
    ini_parser.c       leitor do config.ini
    msc_disk.c         modo EDIÇÃO: pendrive USB sobre a partição FAT
    led.c              LED de status
    rev7_map.h         tabela de comandos (gerada da captura USB)
    idf_component.yml  dependências gerenciadas (esp_tinyusb, led_strip)
    CMakeLists.txt
  storage/config.ini   config padrão embutido na primeira inicialização
  partitions.csv       partição FAT "storage" para o config.ini
  sdkconfig.defaults   S3, 8MB flash, PSRAM, TinyUSB MSC, rótulo FATFS, suporte a hub
```

## Solução de problemas

| Sintoma | Causa | Correção |
|---|---|---|
| LED fica piscando âmbar, nunca fixo | controladora não vista = sem VBUS | use um adaptador OTG **com energia**; controladora na **placa**, não no PC |
| LED vermelho no início | falha ao montar o FAT | `idf.py erase-flash` e grave de novo |
| A porta serial some quando pisca âmbar | porta USB única: o modo host a assume | normal; uma placa com porta UART (bridge) separada mostra o log do host |
| `requirements not satisfied` (verificador do IDF) | IDF antigo + Python novo | use ESP-IDF **5.5** + Python **3.12** |
| `ld: tapi error ... MacOSX27.0.sdk` | SDK do macOS quebrado | `export SDKROOT=.../MacOSX26.5.sdk` |
| `CONFIG_TINYUSB_MSC_BUFSIZE ... must be >= WL_SECTOR_SIZE` | buffer < setor | `CONFIG_TINYUSB_MSC_BUFSIZE=4096` (já configurado) |
