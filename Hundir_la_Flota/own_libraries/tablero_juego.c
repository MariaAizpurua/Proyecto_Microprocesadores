/*
 * tablero_juego.c
 * Reglas de disparo y actualizacion de los tableros.
 */

#include <stdint.h>
#include <stdbool.h>
#include <own_libraries/tablero.h>
#include <own_libraries/enlace_juego.h>
#include <own_libraries/tablero_juego.h>
bool TABLERO_flota_hundida(uint8_t t[FILAS_TABLERO][COLS_TABLERO])
{
    uint8_t x, y;
    for (y = 0; y < FILAS_TABLERO; y++)
        for (x = 0; x < COLS_TABLERO; x++)
            if (ES_BARCO_INTACTO(t[y][x]))
                return false;
    return true;
}

uint8_t TABLERO_resolver_disparo(uint8_t flota[FILAS_TABLERO][COLS_TABLERO],
                                uint8_t casilla)
{
    uint8_t fila = casilla / COLS_TABLERO;
    uint8_t col = casilla % COLS_TABLERO;
    uint8_t valor = flota[fila][col];
    uint8_t x, y, restantes = 0;

    if (valor == AGUA)
    {
        flota[fila][col] = BOMBA_FALLIDA;
        return RES_AGUA;
    }

    if (!ES_BARCO_INTACTO(valor))
        return RESULTADO_INVALIDO;

    flota[fila][col] = valor + OFFSET_TOCADO;
    for (y = 0; y < FILAS_TABLERO; y++)
        for (x = 0; x < COLS_TABLERO; x++)
            if (flota[y][x] == valor)
                restantes++;

    return restantes ? valor : (uint8_t)(valor + OFFSET_TOCADO);
}

void TABLERO_actualizar_objetivo(uint8_t objetivo[FILAS_TABLERO][COLS_TABLERO],
                                uint8_t casilla, uint8_t resultado)
{
    uint8_t fila = casilla / COLS_TABLERO;
    uint8_t col = casilla % COLS_TABLERO;
    uint8_t barco, x, y;

    if (resultado == RES_AGUA)
    {
        objetivo[fila][col] = BOMBA_FALLIDA;
        return;
    }

    if (resultado >= 1 && resultado <= NUM_BARCOS)
    {
        objetivo[fila][col] = resultado + OFFSET_TOCADO;
        return;
    }

    if (resultado > NUM_BARCOS && resultado <= 2 * NUM_BARCOS)
    {
        barco = resultado - NUM_BARCOS;
        objetivo[fila][col] = HUNDIDO;
        for (y = 0; y < FILAS_TABLERO; y++)
            for (x = 0; x < COLS_TABLERO; x++)
                if (objetivo[y][x] == barco + OFFSET_TOCADO)
                    objetivo[y][x] = HUNDIDO;
    }
}

bool TABLERO_flota_rival_hundida(uint8_t objetivo[FILAS_TABLERO][COLS_TABLERO])
{
    uint8_t x, y, hundidas = 0;
    for (y = 0; y < FILAS_TABLERO; y++)
        for (x = 0; x < COLS_TABLERO; x++)
            if (objetivo[y][x] == HUNDIDO)
                hundidas++;
    return hundidas == 17; /* longitud total: 2+3+3+4+5 */
}
