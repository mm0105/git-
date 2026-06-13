#ifndef __PWM_H
#define __PWM_H

#include "settings.h"

/* 启动 TIM2_CH2 PWM 输出（MX_TIM2_Init 之后调用一次） */
void pwm_init(void);

/* 把当前 D_actual 写到 TIM2 CCR（写前不再做斜坡） */
void pwm_apply_duty(void);

/* 1 秒一次的斜坡逻辑：手动追 TarD / 自动按 P-TarP 关系调 D_actual
 * 由 timer_task 每 PWM_STEP_MS 调用一次 */
void pwm_ramp_tick(void);

#endif /* __PWM_H */
