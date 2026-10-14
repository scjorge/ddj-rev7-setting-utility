# DDJ-REV7 — Mapa de comandos USB (com rótulos)

Extraído de `usb-rev7-full.pcapng`. Cada comando é uma transferência de controle USB **sem dados** (só o setup packet de 8 bytes).

## Dispositivo

| idVendor | idProduct | endpoint | transferência |
|---|---|---|---|
| `0x2B73` (11123) | `0x003B` (59) | 0 (control) | OUT · Vendor · Device |

## Formato comum

```
bmRequestType=0x40  bRequest=0x03  wLength=0
ctrl_transfer(0x40, 3, wValue, wIndex, None)
setup packet: 40 03 <wValueLo> <wValueHi> <wIndexLo> <wIndexHi> 00 00
```

> **Rótulos marcados com ⚠️**: o texto é imagem na interface do app, então deduzi o significado pelo nome interno do controle. Confirme na tela qual valor corresponde a qual rótulo — a ordem (valor 0 vs 1) pode estar invertida.

---

## 1. Roteamento DDJ OUTPUT — `0xC002`  ✅ verificado

`wValue = (porta<<8) | código`. Porta 1=USB1/2, 2=USB3/4, 3=USB5/6.

| wValue | setup packet | porta | rota |
|---|---|---|---|
| `0x010A` | `40 03 0A 01 02 C0 00 00` | USB1/2 | MIX (REC OUT with MIC) |
| `0x0115` | `40 03 15 01 02 C0 00 00` | USB1/2 | MIX (REC OUT without MIC) |
| `0x0111` | `40 03 11 01 02 C0 00 00` | USB1/2 | Post CH1 Fader |
| `0x0107` | `40 03 07 01 02 C0 00 00` | USB1/2 | CROSSFADER A |
| `0x0108` | `40 03 08 01 02 C0 00 00` | USB1/2 | CROSSFADER B |
| `0x0109` | `40 03 09 01 02 C0 00 00` | USB1/2 | MIC |
| `0x010D` | `40 03 0D 01 02 C0 00 00` | USB1/2 | AUX |
| `0x0103` | `40 03 03 01 02 C0 00 00` | USB1/2 | Control Tone |
| `0x020A` | `40 03 0A 02 02 C0 00 00` | USB3/4 | MIX (REC OUT with MIC) |
| `0x0215` | `40 03 15 02 02 C0 00 00` | USB3/4 | MIX (REC OUT without MIC) |
| `0x0212` | `40 03 12 02 02 C0 00 00` | USB3/4 | Post CH2 Fader |
| `0x0207` | `40 03 07 02 02 C0 00 00` | USB3/4 | CROSSFADER A |
| `0x0208` | `40 03 08 02 02 C0 00 00` | USB3/4 | CROSSFADER B |
| `0x0209` | `40 03 09 02 02 C0 00 00` | USB3/4 | MIC |
| `0x020D` | `40 03 0D 02 02 C0 00 00` | USB3/4 | AUX |
| `0x0203` | `40 03 03 02 02 C0 00 00` | USB3/4 | Control Tone |
| `0x0315` | `40 03 15 03 02 C0 00 00` | USB5/6 | MIX (REC OUT without MIC) |
| `0x0307` | `40 03 07 03 02 C0 00 00` | USB5/6 | CROSSFADER A |
| `0x0308` | `40 03 08 03 02 C0 00 00` | USB5/6 | CROSSFADER B |
| `0x0309` | `40 03 09 03 02 C0 00 00` | USB5/6 | MIC |
| `0x030D` | `40 03 0D 03 02 C0 00 00` | USB5/6 | AUX |
| `0x0311` | `40 03 11 03 02 C0 00 00` | USB5/6 | Post CH1 Fader |
| `0x0312` | `40 03 12 03 02 C0 00 00` | USB5/6 | Post CH2 Fader |
| `0x030A` | `40 03 0A 03 02 C0 00 00` | USB5/6 | MIX (REC OUT with MIC) |

## 2. Nível de saída USB (MaxLevel) — `0xC003`  ✅ verificado

| wValue | setup packet | nível |
|---|---|---|
| `0x0200` | `40 03 00 02 03 C0 00 00` | -10 dB |
| `0x0100` | `40 03 00 01 03 C0 00 00` | -15 dB |
| `0x0000` | `40 03 00 00 03 C0 00 00` | -19 dB |
| `0x0300` | `40 03 00 03 03 C0 00 00` | -5 dB |

## 3. Demais ajustes (com rótulos)

### 4-Beat Loop Button Type — `0xC212`  ⚠️ confira no app

| wValue | setup packet | rótulo |
|---|---|---|
| `0x0100` | `40 03 00 01 12 C2 00 00` | tipo B |
| `0x0000` | `40 03 00 00 12 C2 00 00` | tipo A |

### 4-Beat Loop Button Length — `0xC223`  ⚠️ confira no app

| wValue | setup packet | rótulo |
|---|---|---|
| `0x0700` | `40 03 00 07 23 C2 00 00` | opção 7 |
| `0x0800` | `40 03 00 08 23 C2 00 00` | 8 |
| `0x0E00` | `40 03 00 0E 23 C2 00 00` | 14 |
| `0x0D00` | `40 03 00 0D 23 C2 00 00` | 13 |
| `0x0C00` | `40 03 00 0C 23 C2 00 00` | 12 |
| `0x0B00` | `40 03 00 0B 23 C2 00 00` | 11 |
| `0x0A00` | `40 03 00 0A 23 C2 00 00` | 10 |

### PAD Mode — `0xC214`  ⚠️ confira no app

| wValue | setup packet | rótulo |
|---|---|---|
| `0x0100` | `40 03 00 01 14 C2 00 00` | modo B |
| `0x0000` | `40 03 00 00 14 C2 00 00` | modo A |

### Crossfader Cut Lag — `0xC211`  ⚠️ confira no app

| wValue | setup packet | rótulo |
|---|---|---|
| `0x0500` | `40 03 00 05 11 C2 00 00` | posição 5 |
| `0x0100` | `40 03 00 01 11 C2 00 00` | posição 1 |
| `0x0200` | `40 03 00 02 11 C2 00 00` | posição 2 |
| `0x0300` | `40 03 00 03 11 C2 00 00` | posição 3 |
| `0x0400` | `40 03 00 04 11 C2 00 00` | posição 4 |
| `0x0600` | `40 03 00 06 11 C2 00 00` | posição 6 |
| `0x0700` | `40 03 00 07 11 C2 00 00` | posição 7 |
| `0x0800` | `40 03 00 08 11 C2 00 00` | posição 8 |
| `0x0900` | `40 03 00 09 11 C2 00 00` | posição 9 |
| `0x0A00` | `40 03 00 0A 11 C2 00 00` | posição 10 |

### Crossfader Fade (curva) — `0xC215`  ✅ verificado

| wValue | setup packet | rótulo |
|---|---|---|
| `0x0300` | `40 03 00 03 15 C2 00 00` | LARGE |
| `0x0000` | `40 03 00 00 15 C2 00 00` | SHARP |
| `0x0100` | `40 03 00 01 15 C2 00 00` | NONE |
| `0x0200` | `40 03 00 02 15 C2 00 00` | SMALL |

### Time Mode — `0xC226`  ⚠️ confira no app

| wValue | setup packet | rótulo |
|---|---|---|
| `0x0000` | `40 03 00 00 26 C2 00 00` | Elapsed (decorrido) |
| `0x0100` | `40 03 00 01 26 C2 00 00` | Remain (restante) |

### Hot Cue Countdown — `0xC227`  ⚠️ confira no app

| wValue | setup packet | rótulo |
|---|---|---|
| `0x0000` | `40 03 00 00 27 C2 00 00` | Off |
| `0x0100` | `40 03 00 01 27 C2 00 00` | On |

### RPM Button Mode — `0xC22D`  ⚠️ confira no app

| wValue | setup packet | rótulo |
|---|---|---|
| `0x0100` | `40 03 00 01 2D C2 00 00` | Long Press |
| `0x0000` | `40 03 00 00 2D C2 00 00` | Instant |

### Slow Speed Stabilizer — `0xC22E`  ⚠️ confira no app

| wValue | setup packet | rótulo |
|---|---|---|
| `0x0100` | `40 03 00 01 2E C2 00 00` | On |
| `0x0000` | `40 03 00 00 2E C2 00 00` | Off |

---

## Exemplo: chave RPM Button Mode

```python
RPM_BUTTON_MODE = {
    "Instant":   0x0000,   # setup: 40 03 00 00 2D C2 00 00
    "Long Press":0x0100,   # setup: 40 03 00 01 2D C2 00 00
}
ctrl_transfer(0x40, 3, RPM_BUTTON_MODE["Instant"], 0xC22D, None)
```

> Observação: este ajuste (Instant / Long Press) é o comportamento do botão de RPM, **não** a troca 33/45 rpm. O 33/45 é o botão físico do deck e não aparece como ajuste no utilitário.

## Sua configuração-alvo

```python
ctrl_transfer(0x40, 3, 0x030A, 0xC002, None)   # USB5/6 = MIX (REC OUT with MIC)
ctrl_transfer(0x40, 3, 0x0300, 0xC003, None)   # nivel = -5 dB
```

## Observações

- Volátil (só RAM): reenviar ao ligar a controladora.
- Por porta USB: enviar na mesma porta que o iPad/computador usará.
- Sem autenticação: o setup packet sozinho aplica.