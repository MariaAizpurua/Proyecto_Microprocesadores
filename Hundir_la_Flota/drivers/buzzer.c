#include <msp430.h>
#include <hal/hal_pwm.h>
#include "buzzer.h"

/* ------------------------------------------------------------------
 * Marcha de guerra (re menor, ~120 pulsos por minuto).
 * Composicion propia: llamada de tambor, subida heroica y final.
 * Cada nota suena su duracion menos un pequeno silencio para que las
 * notas repetidas se oigan separadas (estilo marcial, staccato).
 * Nota: el PWM no baja de ~245 Hz (periodo de 16 bits a 16 MHz), por
 * eso la nota mas grave es RE4.
 * ------------------------------------------------------------------ */
#define SIL   0
#define RE4   294
#define MI4   330
#define FA4   349
#define SOL4  392
#define LA4   440
#define SIb4  466
#define DO5   523
#define DOs5  554
#define RE5   587
#define MI5   659
#define FA5   698
#define SOL5  784
#define LA5   880

#define SEMI  125    /* semicorchea */
#define CORC  250    /* corchea */
#define CORP  375    /* corchea con puntillo */
#define NEGR  500    /* negra */
#define NEGP  750    /* negra con puntillo */
#define BLAN  1000   /* blanca */

#define HUECO_MS 30  /* silencio al final de cada nota */

typedef struct
{
    uint16_t freq;
    uint16_t ms;
} nota_t;

static const nota_t melodia[] = {
    /* Llamada de tambor */
    {RE4, SEMI}, {RE4, SEMI}, {RE4, CORC}, {LA4, NEGR},
    {RE4, SEMI}, {RE4, SEMI}, {RE4, CORC}, {SIb4, NEGR},
    {LA4, CORC}, {SOL4, CORC}, {FA4, CORC}, {MI4, CORC},
    {RE4, NEGP}, {SIL, CORC},

    /* Subida heroica */
    {FA4, CORC}, {LA4, CORC}, {RE5, NEGP}, {DO5, CORC},
    {SIb4, CORC}, {LA4, CORC}, {SOL4, CORC}, {SIb4, CORC},
    {LA4, BLAN},
    {FA4, CORC}, {LA4, CORC}, {RE5, NEGP}, {MI5, CORC},
    {FA5, NEGR}, {MI5, CORC}, {DOs5, CORC},
    {RE5, BLAN},

    /* Climax */
    {LA5, NEGP}, {SOL5, CORC}, {FA5, NEGR}, {MI5, NEGR},
    {FA5, CORP}, {MI5, SEMI}, {RE5, CORC}, {DO5, CORC}, {SIb4, NEGR},
    {SOL4, CORC}, {SIb4, CORC}, {RE5, NEGP}, {DO5, CORC},
    {SIb4, NEGR}, {LA4, NEGR}, {DOs5, NEGR},

    /* Final y redoble */
    {RE5, CORP}, {LA4, SEMI}, {RE5, CORP}, {LA4, SEMI},
    {RE5, BLAN},
    {RE4, SEMI}, {RE4, SEMI}, {RE4, SEMI}, {RE4, SEMI}, {RE4, NEGR},
    {SIL, NEGR}
};

#define NUM_NOTAS (sizeof(melodia) / sizeof(melodia[0]))

static bool musica_activa;
static bool sonando;
static uint16_t musica_tiempo_ms;
static uint8_t musica_nota;

static void tocar_nota(uint8_t n)
{
    HAL_PWM_update_buzzer_freq(melodia[n].freq);
    sonando = (melodia[n].freq != SIL);
}

void BUZ_sound_on()
{
    HAL_PWM_update_buzzer_freq(1000);
}

void BUZ_sound_off()
{
    HAL_PWM_update_buzzer_freq(0);
}

void BUZ_music_set(bool enabled)
{
    musica_activa = enabled;
    musica_tiempo_ms = 0;
    musica_nota = 0;
    if (enabled)
        tocar_nota(0);
    else
    {
        HAL_PWM_update_buzzer_freq(0);
        sonando = false;
    }
}

void BUZ_music_tick(uint16_t elapsed_ms)
{
    if (!musica_activa)
        return;

    musica_tiempo_ms += elapsed_ms;

    /* Siguiente nota */
    while (musica_tiempo_ms >= melodia[musica_nota].ms)
    {
        musica_tiempo_ms -= melodia[musica_nota].ms;
        musica_nota = (musica_nota + 1) % NUM_NOTAS;
        tocar_nota(musica_nota);
    }

    /* Pequeno silencio al final de la nota (articulacion) */
    if (sonando && musica_tiempo_ms + HUECO_MS >= melodia[musica_nota].ms)
    {
        HAL_PWM_update_buzzer_freq(0);
        sonando = false;
    }
}

void BUZ_wait_ms(uint16_t ms)
{
    while (ms--)
    {
        __delay_cycles(16000);   /* 1 ms a 16 MHz */
        BUZ_music_tick(1);
    }
}
