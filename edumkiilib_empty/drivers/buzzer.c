#include <hal/hal_pwm.h>
#include "buzzer.h"

static bool musica_activa;
static uint16_t musica_tiempo_ms;
static uint8_t musica_nota;
static const uint16_t melodia[] = {
    523, 659, 784, 659, 587, 698, 880, 698,
    523, 659, 784, 1047, 784, 659, 523, 0
};

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
    HAL_PWM_update_buzzer_freq(enabled ? melodia[0] : 0);
}

void BUZ_music_tick(uint16_t elapsed_ms)
{
    if (!musica_activa)
        return;

    musica_tiempo_ms += elapsed_ms;
    while (musica_tiempo_ms >= 220)
    {
        musica_tiempo_ms -= 220;
        musica_nota = (musica_nota + 1) % (sizeof(melodia) / sizeof(melodia[0]));
        HAL_PWM_update_buzzer_freq(melodia[musica_nota]);
    }
}
