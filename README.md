# 🚢💥 Hundir la Flota v1.0

**Autores:** María Aizpurua, Irene Alberdi, Ander Rodríguez, Jon Xabin Urretabizkaia

## 📋 Descripción general

Este proyecto implementa el juego **Hundir la Flota** mediante un sistema de **comunicación bidireccional simétrico**. Ambas estaciones utilizan exactamente el mismo código fuente, diferenciándose únicamente por el identificador de jugador configurado antes de la compilación (**Jugador 0** y **Jugador 1**).

## 🛠️ Requisitos de Hardware

Para montar una partida completa (2 jugadores) se necesita por duplicado el siguiente equipamiento:

* **2x** Placa de desarrollo `MSP-EXP430FR5994`

* **2x** Módulo BoosterPack `BOOSTXL-EDUMKII`

* **2x** Placa de radio `LAUNCHXL-CC1310`

## ⚙️ Configuración y carga del código

### 1. Controlador principal (`Hundir_la_Flota`)

El proyecto `Hundir_la_Flota` está preparado para compilarse y cargarse directamente en la placa **MSP-EXP430FR5994** con el módulo **BOOSTXL-EDUMKII**.

* **Jugador 0:** Compilar y cargar el proyecto tal cual viene por defecto.

* **Jugador 1:** Abrir el archivo `main.c`, ir a la **línea 64** y modificar la definición de `JUGADOR_LOCAL` de `0` a `1`. Posteriormente, compilar y cargar en un montaje de hardware idéntico al del Jugador 0.

```c
// Configuración para el Jugador 0 (por defecto)
#define JUGADOR_LOCAL 0

// Configuración para el Jugador 1
#define JUGADOR_LOCAL 1
```

### 2. Módulo de radio (`cc1310_radio/trx`)

Para habilitar la comunicación **Half-Duplex** entre ambos jugadores:

1. Localiza la carpeta `cc1310_radio` y accede al subdirectorio `trx`.

2. Compila y carga ese proyecto en las dos placas **LAUNCHXL-CC1310**.

3. Conecta una placa `CC1310` a cada estación de jugador siguiendo el esquema de conexiones de la siguiente sección.

## 🔌 Esquema de conexiones (UART)

Conecta cada módulo `BOOSTXL-EDUMKII` a su correspondiente placa `LAUNCHXL-CC1310` de la siguiente manera:

| Pin en BOOSTXL-EDUMKII | Dirección | Pin en LAUNCHXL-CC1310 | 
| ----- | ----- | ----- | 
| **`J1.4`** | $\rightarrow$ | **`RXD`** | 
| **`J1.3`** | $\rightarrow$ | **`TXD`** | 
| **`J2.0`** | $\leftrightarrow$ | **`GND`** | 

## 🚦 Diagnóstico y código de LEDs (CC1310)

Las placas `LAUNCHXL-CC1310` utilizan su pareja de LEDs integrados para indicar en tiempo real el estado de la conexión UART-Radio:

| Estado del LED | Significado | 
| ----- | ----- | 
| 🟢 **Verde encendido (fijo)** | La radio está lista y la conexión UART-Radio se ha configurado correctamente. | 
| 🟢 **Verde parpadeando** | Se está recibiendo una trama por radio. | 
| 🔴 **Rojo encendido** | Se está enviando una trama por radio. | 
| 🔴🟢 **Rojo y verde parpadeando** | **Error** en la configuración UART-Radio. | 