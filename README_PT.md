# DDJ-REV7 Setting Utility

Para utilização em apps fora do escopo do Windows ou Mac, não existe o APP **DDJ REV 7 Setting Utility**. O que torna impossível configurar coisas importantes como o mapeamento de áudio dos canais 5/6 da saída de áudio, que é o que permite softwares como o **DJay Pro** gravar a sua mix.

A própria REV 7 limpa toda a memória quando é desligada e perde todas as informações salvas. Além disso, o próprio iOS bloqueia a criação de apps com permissão para enviar os sinais USB necessários. Então, pensando nisso, criei um firmware para um microcomputador externo. Na prática, basta conectar o DDJ REV 7 na placa antes, aguardar a atualização e em seguida conectar normalmente no iOS.

> **OBS 1:** Somente as opções de efeitos não são mapeáveis.
>
> **OBS 2:** Realizei o projeto utilizando a placa **ESP32-S3-N8R2**. Caso queira contribuir com informações de utilização em outros modelos, sinta-se à vontade para enviar uma PR.

🇬🇧 [English version](README.md)

---

## Como foi feito

O projeto foi feito monitorando as atividades USB do utilitário oficial e replicando cada evento (control transfer) para a REV 7. O mapa completo de comandos está em [`docs/usb-command-map.md`](docs/usb-command-map.md).

## Como funciona

1. Você conecta o DDJ-REV7 **na placa** (microcomputador) e liga.
2. A placa detecta a controladora e envia os ajustes do `config.ini` por USB — os mesmos que o utilitário oficial enviaria.
3. Em seguida você passa o cabo para o iOS/PC e usa normalmente (os ajustes ficam até a controladora ser desligada).

O `config.ini` é editável: a placa também aparece como um pendrive USB.

## Placas suportadas

| Placa | Status | Guia |
|---|---|---|
| **ESP32-S3** (testado na ESP32-S3-N8R2) | ✅ funcionando | [boards/esp32-s3](boards/esp32-s3/README_PT.md) |
| Raspberry Pi (Linux) | 🧩 planejado / contribuições | [boards/raspberry-pi](boards/raspberry-pi/README_PT.md) |

> Quer portar para outra placa? Veja a seção **Contribuindo** abaixo.

## Estrutura do repositório

```
ddj-rev7-usb-config/
  README.md / README_PT.md     visão geral do projeto (este arquivo)
  docs/                        protocolo USB (independente de placa)
    usb-command-map.md           mapa de comandos (legível)
    usb-commands.json            comandos em JSON
  config/
    config.ini                   configuração canônica de referência
  boards/
    esp32-s3/                    firmware ESP-IDF para ESP32-S3  (✅)
      README.md / README_PT.md
      main/ storage/ ...
    raspberry-pi/                espaço para versão em Raspberry Pi
      README.md / README_PT.md
```

## Documentação

- **Protocolo / mapa de comandos:** [`docs/usb-command-map.md`](docs/usb-command-map.md) e [`docs/usb-commands.json`](docs/usb-commands.json).
- **Configuração de referência:** [`config/config.ini`](config/config.ini).
- **Guia por placa:** veja a pasta da sua placa em `boards/`.

## Contribuindo

Para adicionar suporte a outra placa:
1. Crie `boards/<sua-placa>/` com o firmware/código daquela plataforma.
2. Reutilize o protocolo de [`docs/`](docs/) e a configuração de [`config/`](config/).
3. Inclua um `README.md` e `README_PT.md` na pasta da placa (ligação, build, uso, LED/indicadores).
4. Abra uma PR. O núcleo (mapa de comandos e `config.ini`) é o mesmo para todas as placas; só muda a camada de USB host de cada plataforma.

## Aviso

Este projeto apenas envia os comandos de configuração documentados por USB, como o app oficial faz — **não modifica o firmware da controladora**. Valide no seu hardware. Use por sua conta e risco.
