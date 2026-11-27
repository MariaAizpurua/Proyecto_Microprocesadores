/*
 * estilo_militar.c
 * Dibujo del menu y de las pantallas de juego.
 */

#include "estilo_militar.h"
#include <grlib.h>

extern Graphics_Context g_sContext;

/*
 * Distribucion de las pantallas (128 x 128):
 *   +------------------------------+
 *   | insignias   TITULO     ancla | cabecera oscura
 *   |         - subtitulo -        |
 *   |==============================| lineas doradas
 *   |                              |
 *   |       contenido / placas     | fondo camuflado
 *   |                              |
 *   |==============================|
 *   |         instruccion          | pie
 *   +------------------------------+
 * Los circulos se dibujan con lineas porque circle.c de GRLIB no se compila.
 */

/* ---------------- Paleta ---------------- */
#define C_FONDO       0x3B4A2AUL   /* verde oliva */
#define C_CAMO_OSC    0x26321BUL   /* oliva oscuro */
#define C_CAMO_TIERRA 0x5C5036UL   /* marron caqui */
#define C_BANDA       0x161D10UL   /* casi negro verdoso */
#define C_ORO         0xD4AF37UL
#define C_ORO_OSC     0x8C7220UL
#define C_KAKI        0xD2C38FUL
#define C_PLACA       0x4B5A33UL
#define C_TEXTO_OSC   0x161D10UL
#define C_ROJO        0xB4231AUL
#define C_ROJO_CLARO  0xF04A3CUL
#define C_VERDE       0x3CD23CUL
#define C_VERDE_BOTON 0x2E6B2AUL
#define C_AZUL        0x1E5A96UL
#define C_AZUL_CLARO  0x78B4F0UL

/* ---------------- Geometria ---------------- */
#define CAB_ALTO     29          /* cabecera: y 0..29 */
#define PIE_Y        112         /* pie: y 112..127 */

/* menu */
#define PLACA_X0     16
#define PLACA_X1     113
#define PLACA_ALTO   18
#define PLACA_Y0     40
#define PLACA_PASO   24
#define MARCA_X0     4           /* flecha de seleccion a la izquierda */
#define MARCA_X1     12
#define PILOTO_X     103         /* piloto de la musica */

/* pantallas de mensaje */
#define MSG_X0       6
#define MSG_X1       121
#define MSG_Y0       38
#define MSG_Y1       60
#define ICONO_Y      78
#define DETALLE_Y    100

static const char *const etiquetas[MENU_NUM_OPCIONES] = {
    "ESTADISTICAS", 0, "A COMBATE"
};

/* ---------------- Primitivas ---------------- */
static void color(uint32_t c)
{
    Graphics_setForegroundColor(&g_sContext, c);
}

static void rect_lleno(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    Graphics_Rectangle r = {x0, y0, x1, y1};
    Graphics_fillRectangle(&g_sContext, &r);
}

static void rect_borde(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    Graphics_Rectangle r = {x0, y0, x1, y1};
    Graphics_drawRectangle(&g_sContext, &r);
}

/* Linea de 2 pixeles de grosor (desplazada hacia abajo) */
static void linea_gruesa(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    Graphics_drawLine(&g_sContext, x0, y0, x1, y1);
    Graphics_drawLine(&g_sContext, x0, y0 + 1, x1, y1 + 1);
}

/* Circulo relleno a base de lineas horizontales (sin circle.c) */
static void disco(int16_t cx, int16_t cy, int16_t r)
{
    int16_t dy, dx;
    for (dy = -r; dy <= r; dy++)
    {
        dx = r;
        while (dx > 0 && dx * dx + dy * dy > r * r)
            dx--;
        Graphics_drawLineH(&g_sContext, cx - dx, cx + dx, cy + dy);
    }
}

/* Estrella de 5 puntas pequena (radio 4) */
static void estrella(int16_t cx, int16_t cy)
{
    static const int8_t px[5] = { 0, 4,  2, -2, -4};
    static const int8_t py[5] = {-4, -1, 3,  3, -1};
    static const uint8_t orden[6] = {0, 2, 4, 1, 3, 0};
    uint8_t i;

    for (i = 0; i < 5; i++)
        Graphics_drawLine(&g_sContext,
                          cx + px[orden[i]],     cy + py[orden[i]],
                          cx + px[orden[i + 1]], cy + py[orden[i + 1]]);
    rect_lleno(cx - 1, cy - 1, cx + 1, cy + 1);
}

/* Texto centrado: usa la fuente grande si cabe, si no la pequena */
static void texto_centrado(const char *s, int16_t x, int16_t y, int16_t ancho_max)
{
    Graphics_setFont(&g_sContext, &g_sFontCm12);
    if (Graphics_getStringWidth(&g_sContext, (int8_t *)s, AUTO_STRING_LENGTH) > ancho_max)
        Graphics_setFont(&g_sContext, &g_sFontFixed6x8);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)s, AUTO_STRING_LENGTH,
                                x, y, TRANSPARENT_TEXT);
}

static void texto_pequeno(const char *s, int16_t x, int16_t y)
{
    Graphics_setFont(&g_sContext, &g_sFontFixed6x8);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)s, AUTO_STRING_LENGTH,
                                x, y, TRANSPARENT_TEXT);
}

/* Placa remachada con sombra */
static void placa(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  uint32_t relleno, uint32_t borde, uint32_t remaches)
{
    color(relleno);
    rect_lleno(x0, y0, x1, y1);
    color(borde);
    rect_borde(x0, y0, x1, y1);
    color(C_BANDA);   /* sombra */
    Graphics_drawLineH(&g_sContext, x0 + 1, x1 + 1, y1 + 1);
    Graphics_drawLineV(&g_sContext, x1 + 1, y0 + 1, y1 + 1);

    color(remaches);
    rect_lleno(x0 + 2, y0 + 2, x0 + 3, y0 + 3);
    rect_lleno(x1 - 3, y0 + 2, x1 - 2, y0 + 3);
    rect_lleno(x0 + 2, y1 - 3, x0 + 3, y1 - 2);
    rect_lleno(x1 - 3, y1 - 3, x1 - 2, y1 - 2);
}

/* ---------------- Insignias ---------------- */

/* Galones de sargento: tres "^" apilados */
static void galones(int16_t cx, int16_t y)
{
    uint8_t i;
    color(C_ORO);
    for (i = 0; i < 3; i++)
    {
        int16_t yy = y + i * 6;
        linea_gruesa(cx - 8, yy + 5, cx, yy);
        linea_gruesa(cx, yy, cx + 8, yy + 5);
    }
}

/* Ancla naval */
static void ancla(int16_t cx, int16_t y)
{
    color(C_ORO);
    rect_borde(cx - 2, y, cx + 2, y + 4);          /* argolla */
    rect_lleno(cx - 6, y + 7, cx + 6, y + 8);      /* cepo */
    rect_lleno(cx - 1, y + 5, cx, y + 21);         /* cana */
    linea_gruesa(cx, y + 21, cx - 5, y + 18);      /* brazos */
    linea_gruesa(cx - 5, y + 18, cx - 8, y + 13);
    linea_gruesa(cx, y + 21, cx + 5, y + 18);
    linea_gruesa(cx + 5, y + 18, cx + 8, y + 13);
    Graphics_drawLine(&g_sContext, cx - 8, y + 13, cx - 10, y + 16);  /* unas */
    Graphics_drawLine(&g_sContext, cx + 8, y + 13, cx + 10, y + 16);
}

/* ---------------- Iconos de las pantallas de mensaje ---------------- */

static void icono_estrellas(void)
{
    color(C_ORO);
    Graphics_drawLineH(&g_sContext, 14, 38, ICONO_Y);
    Graphics_drawLineH(&g_sContext, 90, 114, ICONO_Y);
    estrella(50, ICONO_Y);
    estrella(64, ICONO_Y - 2);
    estrella(78, ICONO_Y);
}

static void icono_olas(void)
{
    uint8_t ola;
    int16_t x;
    color(C_AZUL_CLARO);
    for (ola = 0; ola < 3; ola++)
    {
        int16_t y = ICONO_Y - 7 + ola * 7;
        for (x = 34; x < 94; x += 6)
        {
            linea_gruesa(x, y, x + 3, y - 3);
            linea_gruesa(x + 3, y - 3, x + 6, y);
        }
    }
}

static void icono_diana(uint32_t c)
{
    color(c);
    disco(64, ICONO_Y, 10);
    color(C_FONDO);
    disco(64, ICONO_Y, 8);
    color(c);
    rect_lleno(50, ICONO_Y, 78, ICONO_Y + 1);      /* cruz */
    rect_lleno(64, ICONO_Y - 14, 65, ICONO_Y + 14);
    disco(64, ICONO_Y, 2);
}

static void icono_aviso(void)
{
    color(C_ORO);
    linea_gruesa(64, ICONO_Y - 12, 50, ICONO_Y + 10);
    linea_gruesa(64, ICONO_Y - 12, 78, ICONO_Y + 10);
    rect_lleno(50, ICONO_Y + 10, 78, ICONO_Y + 11);
    rect_lleno(63, ICONO_Y - 4, 65, ICONO_Y + 3);  /* "!" */
    rect_lleno(63, ICONO_Y + 6, 65, ICONO_Y + 7);
}

/* ---------------- Partes comunes ---------------- */

static void camuflaje(void)
{
    /* Manchas fuera de la columna de la flecha y del icono central */
    color(C_CAMO_OSC);
    disco(122, 46, 6);
    disco(4, 61, 3);
    disco(124, 84, 7);
    disco(40, 61, 4);
    disco(98, 92, 4);
    disco(6, 108, 5);
    color(C_CAMO_TIERRA);
    disco(118, 66, 4);
    disco(70, 37, 3);
    disco(20, 85, 4);
    disco(126, 104, 4);
}

static void cabecera(const char *titulo, const char *subtitulo)
{
    int16_t medio;

    color(C_BANDA);
    rect_lleno(0, 0, 127, CAB_ALTO);
    color(C_ORO);
    Graphics_drawLineH(&g_sContext, 0, 127, CAB_ALTO + 1);
    Graphics_drawLineH(&g_sContext, 0, 127, CAB_ALTO + 3);

    galones(14, 5);
    ancla(114, 4);

    Graphics_setFont(&g_sContext, &g_sFontCm12);
    color(C_ORO);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)titulo, AUTO_STRING_LENGTH,
                                64, 9, TRANSPARENT_TEXT);
    medio = Graphics_getStringWidth(&g_sContext, (int8_t *)titulo, AUTO_STRING_LENGTH) / 2;
    estrella(64 - medio - 7, 9);
    estrella(64 + medio + 7, 9);

    color(C_KAKI);
    texto_pequeno(subtitulo, 64, 22);
}

static void pie(const char *instruccion)
{
    color(C_ORO);
    Graphics_drawLineH(&g_sContext, 0, 127, PIE_Y - 2);
    color(C_BANDA);
    rect_lleno(0, PIE_Y, 127, 127);
    color(C_KAKI);
    texto_pequeno(instruccion, 64, 120);
}

static void fondo(const char *titulo, const char *subtitulo, const char *instruccion)
{
    Graphics_setBackgroundColor(&g_sContext, C_FONDO);
    Graphics_clearDisplay(&g_sContext);
    camuflaje();
    cabecera(titulo, subtitulo);
    pie(instruccion);
}

/* ---------------- Menu ---------------- */

void menu_militar_placa(uint8_t indice, bool seleccionada, bool musica_activa)
{
    int16_t y0 = PLACA_Y0 + indice * PLACA_PASO;
    int16_t y1 = y0 + PLACA_ALTO - 1;
    int16_t yc = y0 + PLACA_ALTO / 2;
    uint32_t c_detalle = seleccionada ? C_TEXTO_OSC : C_KAKI;
    const char *texto;
    int16_t texto_x = 64;

    if (indice >= MENU_NUM_OPCIONES)
        return;

    placa(PLACA_X0, y0, PLACA_X1, y1,
          seleccionada ? C_ORO : C_PLACA,
          seleccionada ? C_ORO_OSC : C_KAKI,
          c_detalle);

    /* Texto (la musica deja sitio al piloto) */
    texto = etiquetas[indice];
    if (indice == 1)
    {
        texto = musica_activa ? "MUSICA: ON" : "MUSICA: OFF";
        texto_x = 58;
    }
    color(c_detalle);
    texto_pequeno(texto, texto_x, yc);

    /* Piloto de la musica: verde = ON, rojo = OFF */
    if (indice == 1)
    {
        color(C_BANDA);
        disco(PILOTO_X, yc, 4);
        color(musica_activa ? C_VERDE : C_ROJO_CLARO);
        disco(PILOTO_X, yc, 3);
    }

    /* Flecha de seleccion a la izquierda (o se borra) */
    color(seleccionada ? C_ORO : C_FONDO);
    linea_gruesa(MARCA_X0, yc - 4, MARCA_X1 - 1, yc - 1);
    linea_gruesa(MARCA_X1 - 1, yc - 1, MARCA_X0, yc + 3);
}

void menu_militar_dibujar(uint8_t opcion, bool musica_activa)
{
    uint8_t i;

    fondo("MENU", "- CUARTEL -", "S1:OK  P5.5:SALIR");
    for (i = 0; i < MENU_NUM_OPCIONES; i++)
        menu_militar_placa(i, i == opcion, musica_activa);
}

/* ---------------- Pantallas intermedias ---------------- */

void pantalla_militar(const char *titulo, const char *detalle,
                      const char *instruccion, estilo_pantalla_t estilo)
{
    uint32_t relleno, borde, texto;

    switch (estilo)
    {
    case ESTILO_AGUA:
        relleno = C_AZUL;  borde = C_KAKI;    texto = C_KAKI;      break;
    case ESTILO_TOCADO:
    case ESTILO_PERDIDO:
    case ESTILO_AVISO:
        relleno = C_ROJO;  borde = C_KAKI;    texto = C_KAKI;      break;
    case ESTILO_HUNDIDO:
    case ESTILO_GANADO:
        relleno = C_ORO;   borde = C_ORO_OSC; texto = C_TEXTO_OSC; break;
    default:
        relleno = C_PLACA; borde = C_ORO;     texto = C_ORO;       break;
    }

    fondo("HUNDIR", "- LA FLOTA -", instruccion);

    placa(MSG_X0, MSG_Y0, MSG_X1, MSG_Y1, relleno, borde, texto);
    color(texto);
    texto_centrado(titulo, 64, (MSG_Y0 + MSG_Y1) / 2, MSG_X1 - MSG_X0 - 10);

    switch (estilo)
    {
    case ESTILO_AGUA:    icono_olas();              break;
    case ESTILO_TOCADO:  icono_diana(C_ROJO_CLARO); break;
    case ESTILO_PERDIDO: icono_diana(C_ROJO_CLARO); break;
    case ESTILO_AVISO:   icono_aviso();             break;
    default:             icono_estrellas();         break;
    }

    color(C_KAKI);
    texto_pequeno(detalle, 64, DETALLE_Y);
}

void pantalla_militar_confirmar(const char *pregunta, const char *instruccion)
{
    fondo("HUNDIR", "- LA FLOTA -", instruccion);

    placa(MSG_X0, MSG_Y0, MSG_X1, MSG_Y1, C_PLACA, C_ORO, C_ORO);
    color(C_ORO);
    texto_centrado(pregunta, 64, (MSG_Y0 + MSG_Y1) / 2, MSG_X1 - MSG_X0 - 10);

    /* Botones SI / NO */
    placa(10, 70, 57, 94, C_VERDE_BOTON, C_KAKI, C_KAKI);
    placa(70, 70, 117, 94, C_ROJO, C_KAKI, C_KAKI);
    color(C_KAKI);
    texto_centrado("SI", 34, 79, 40);
    texto_pequeno("S1", 34, 89);
    texto_centrado("NO", 94, 79, 40);
    texto_pequeno("S2", 94, 89);
}
