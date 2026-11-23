# Enlace MSP430–CC1310 a 864,5 MHz

## Puente bidireccional (`trx/`) — recomendado

`trx/` es **un solo programa que se graba en los dos CC1310**. Escucha la radio todo el rato; cuando su MSP430 le manda una trama por la UART, corta la escucha, la transmite y vuelve a escuchar. Lo que llega por radio lo pasa tal cual a su MSP430. Así los dos MSP430 pueden mandar y recibir.

- Proyecto CCS: `trx\ccs\rfEasyLinkTrx_CC1310_LAUNCHXL_nortos_ccs.projectspec`
- Trama: 5 bytes empezando por `0xAA` (el puente no mira el resto).
- LEDs: **verde** se enciende cuando la radio está lista y cambia con cada trama que llega por radio; **rojo** cambia con cada trama que se envía por radio; **rojo parpadeando rápido** = error al configurar UART/radio.
- Si las dos placas transmiten exactamente a la vez, esas tramas se pierden (no hay confirmación).

Cableado de **cada** pareja MSP430 + CC1310 (las dos iguales):

| MSP430 | CC1310 |
|---|---|
| P6.0 (UCA3 TX) | DIO2 (RXD) |
| P6.1 (UCA3 RX) | DIO3 (TXD) |
| GND | GND |

**Importante:** quita los jumpers RXD y TXD de la LaunchPad CC1310. Si conectas el cable en la fila de jumpers, usa el pin del lado **contrario al USB** (el del lado del USB va al chip de depuración XDS110, no al CC1310). Guíate por la serigrafía DIO2/DIO3, no por el número de pin.

## Versión unidireccional (`tx/` y `rx/`)

Estos dos proyectos parten de los ejemplos oficiales EasyLink `nortos` para `CC1310_LAUNCHXL` y llevan las tramas existentes de 5 bytes (`AA`, comando, jugador, resultado, barco) entre los MSP430.

## SDK detectado en este ordenador

La carpeta `C:\ti\simplelink_cc13x0_sdk_4_20_02_07` contiene dentro otra carpeta llamada `simplelink_cc13x0_sdk_4_10_01_01`. Por tanto, el SDK que está instalado realmente es **4.10.01.01**, no 4.20.02.07. Para que CCS descubra este SDK, selecciona la carpeta interior:

`C:\ti\simplelink_cc13x0_sdk_4_20_02_07\simplelink_cc13x0_sdk_4_10_01_01`

En CCS: **Window → Preferences → Code Composer Studio → Products → Install** y selecciona esa ruta. Reinicia CCS después.

## Importar y compilar

Importa estos dos proyectos CCS desde las carpetas `ccs`:

- TX: `tx\ccs\rfEasyLinkTx_CC1310_LAUNCHXL_nortos_ccs.projectspec`
- RX: `rx\ccs\rfEasyLinkRx_CC1310_LAUNCHXL_nortos_ccs.projectspec`

Importa ambos en el mismo workspace. Compila TX y RX. Para programar una placa, selecciona el proyecto correspondiente y pulsa **Debug**; para usar la misma CC1310 como la otra función, programa después el otro proyecto.

## Cableado UART con los MSP430

Usa dos conjuntos: MSP430 de juego + CC1310 TX en un extremo, y CC1310 RX + MSP430 marcador en el otro. Conecta señales cruzadas y comparte GND:

| Señal | Desde | Hacia |
|---|---|---|
| Datos juego → TX radio | MSP430 de juego P6.0 / UCA3 TX | CC1310 TX RXD, DIO2 (jumper RXD, lado contrario al USB) |
| Datos RX radio → marcador | CC1310 RX TXD, DIO3 (jumper TXD, lado contrario al USB) | MSP430 marcador P6.1 / UCA3 RX |
| Referencia | GND de ambas placas | GND común |

Las señales UART son de 3,3 V. No conectes 5 V ni alimentes una placa desde el pin 3V3 de la otra. La UART está configurada a 115200, 8N1.

En el proyecto MSP430 compila `MODO_MARCADOR 0` para la placa de juego y `MODO_MARCADOR 1` para la del marcador. La radio TX transmite cada trama que empieza por `0xAA`; el RX la reenvía sin cambiarla.

## Prueba

1. Programa una CC1310 con el proyecto RX y déjala encendida.
2. Programa la otra CC1310 con el proyecto TX.
3. Programa el MSP430 marcador con `MODO_MARCADOR 1` y conecta CC1310 RX DIO3 a P6.1.
4. Programa el MSP430 de juego con `MODO_MARCADOR 0` y conecta P6.0 a CC1310 TX DIO2.
5. Enciende primero el receptor. Inicia una partida y comprueba que cambian turno y contadores del marcador.

Para aislar un problema, primero prueba los dos CC1310 con ejemplos TX/RX EasyLink y después añade los MSP430. Los dos CC1310 de este puente usan la misma configuración EasyLink del ejemplo y sintonizan `864500000` Hz (864,5 MHz).

El SDK 4.10 es el que está presente en la carpeta actual; si instalas el 4.20.02.07 recomendado por la guía, importa los ejemplos de esa instalación y aplica allí estos dos archivos fuente en sus respectivos proyectos TX/RX.

## Indicadores LED de radio

- CC1310 TX: rojo fijo = entró en el firmware TX; verde fijo = transmitió correctamente al menos una trama RF.
- CC1310 RX: verde fijo = entró en el firmware RX; rojo fijo = recibió una trama RF válida y la escribió por UART.

Al probar, verde sin rojo apunta al enlace RF/recepción; si ambos parpadean pero el marcador no cambia, revisa el UART de salida desde la CC1310 RX hacia el MSP430 marcador.
