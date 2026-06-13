#include "pwm.h"
#include "tim.h"
#include "settings.h"

/* 启动 PWM 输出，并把 D_actual 设为 5% 初始值 */
void pwm_init(void)
{
    D_actual = 5;
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, D_actual);

    /* PA7 舵机 PWM：50Hz TIM17_CH1，与 PA1 同步
     * 初始脉冲 1.5ms（中位 90°），D_actual 同步会立刻覆盖 */
    HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
    __HAL_TIM_SetCompare(&htim17, TIM_CHANNEL_1, 1500);
}

/* 把当前 D_actual 推到 PWM 寄存器（CCR）
 * PA1 (TIM2_CH2):  5%-95% 直接写 D_actual
 * PA7 (TIM17_CH1): 50Hz 舵机，把 D_actual 线性映射到 500-2500us 脉冲
 *                   公式: CCR = 500 + (D_actual - 5) * 200 / 9 */
void pwm_apply_duty(void)
{
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, D_actual);

    uint16_t servo_ccr = (uint16_t)(500u + ((uint32_t)(D_actual - 5) * 200u) / 9u);
    __HAL_TIM_SetCompare(&htim17, TIM_CHANNEL_1, servo_ccr);
}

/* 1%/秒斜坡 + MAN/AUTO 算法 */
void pwm_ramp_tick(void)
{
    if (B2_flag1 == 1)
    {
        /* 手动模式：D_actual 1%/秒 逼近 TarD_value */
        if (D_actual < TarD_value)      D_actual++;
        else if (D_actual > TarD_value) D_actual--;
        if (D_actual == TarD_value) tar_changed = 0;   /* 到位：灭 LD4 */
    }
    else
    {
        /* 自动模式：根据 P 与 TarP 的关系，1%/秒 调 D_actual */
        if (P_value < (TarP_value - 0.5f))
        {
            if (D_actual < 95) D_actual++;
        }
        else if (P_value > (TarP_value + 0.5f))
        {
            if (D_actual > 5)  D_actual--;
        }
        if (P_value >= (TarP_value - 0.5f) && P_value <= (TarP_value + 0.5f))
            tar_changed = 0;                           /* 死区内：灭 LD4 */
    }

    pwm_apply_duty();
}
