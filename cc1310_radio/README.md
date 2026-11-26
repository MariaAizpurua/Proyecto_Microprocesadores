# Enlace MSP430–CC1310 a 864,5 MHz

## Puente bidireccional

El proyecto `trx/` se graba en las dos placas LAUNCHXL-CC1310. Cada MSP430 se conecta a una CC1310; el puente envía por radio las tramas UART recibidas y entrega al MSP430 las tramas que recibe por radio.

- Proyecto CCS: `trx\ccs\rfEasyLinkTrx_CC1310_LAUNCHXL_nortos_ccs.projectspec`
- Frecuencia: 864,5 MHz.
- Tramas: 5 bytes, con `0xAA` como byte inicial. El puente RF las reenvía sin interpretar sus campos; el protocolo de la partida está en `own_libraries/enlace_juego.h`.
- UART: 115200 baudios, 8N1.
- LED verde: queda encendido cuando la radio está lista y cambia al recibir una trama RF válida.
- LED rojo: cambia cuando se transmite una trama; parpadeo rápido indica un error de inicialización.

Si ambos extremos transmiten exactamente a la vez, puede perderse una trama. El protocolo de la partida confirma los resultados y reintenta los mensajes que no reciben respuesta.

## Cableado

Conecta cada MSP430 a su CC1310 de la misma manera:

| MSP430FR5994 | LAUNCHXL-CC1310 |
|---|---|
| P6.0 (UCA3 TX) | DIO2 (RXD) |
| P6.1 (UCA3 RX) | DIO3 (TXD) |
| GND | GND |

Quita los jumpers RXD y TXD de la CC1310. Si conectas los cables en la fila de jumpers, usa los pines del lado contrario al USB. Comprueba las señales DIO2 y DIO3 en la serigrafía.

## Programación y prueba

1. Importa el proyecto `trx` en CCS y compílalo.
2. Programa las dos CC1310 con el mismo firmware `trx`.
3. En el proyecto MSP430, deja `JUGADOR_LOCAL` en `0` para una placa y ponlo en `1` para la otra. Compila y programa cada MSP430.
4. Conecta cada pareja MSP430–CC1310 según la tabla y enciende las cuatro placas.
5. Coloca las flotas e inicia la partida en ambas pantallas.

Los ejemplos `tx/` y `rx/` son pruebas unidireccionales separadas; el juego bidireccional usa `trx/`.

## SDK

La carpeta `C:\ti\simplelink_cc13x0_sdk_4_20_02_07` contiene dentro otra carpeta llamada `simplelink_cc13x0_sdk_4_10_01_01`. Si CCS no detecta el SDK, selecciona la carpeta interior desde **Window → Preferences → Code Composer Studio → Products → Install** y reinicia CCS.
