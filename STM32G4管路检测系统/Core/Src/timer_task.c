#include "timer_task.h"
#include "key.h"
#include "adc_sensor.h"
#include "pwm.h"
#include "flow.h"
#include "led.h"
#include "settings.h"

/* TIM3 1ms 周期中断里调用的总入口
 * 内部按 HAL_GetTick 切分 50ms / 100ms / 1000ms 三个子周期
 *
 * 流程（按周期从短到长）：
 *   - 每次都跑：key_scan
 *   - 50ms 一次：led_tick
 *   - 100ms 一次：adc_get_pressure + flow_accumulate_q
 *   - 1000ms 一次：pwm_ramp_tick */
void timer_task_dispatch(void)
{
    static uint32_t tick_led = 0;
    static uint32_t tick_q   = 0;
    static uint32_t tick_d   = 0;

    uint32_t now = HAL_GetTick();

    /* 1) 按键扫描：每次 TIM3 中断都跑 */
    key_scan();

    /* 2) LED 状态判定：每 50ms */
    if (now - tick_led >= LED_SCAN_MS)
    {
        tick_led = now;
        led_tick();
    }

    /* 3) 压力采样 + 流量累计：每 100ms */
    if (now - tick_q >= Q_SAMPLE_MS)
    {
        tick_q = now;
        adc_get_pressure();
        flow_accumulate_q();
    }

    /* 4) PWM 斜坡：每 1000ms（1%/秒） */
    if (now - tick_d >= PWM_STEP_MS)
    {
        tick_d = now;
        pwm_ramp_tick();
    }
}
