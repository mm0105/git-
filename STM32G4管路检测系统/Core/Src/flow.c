#include "flow.h"
#include "tim.h"
#include "settings.h"

/* TIM16 启动输入捕获（CH1 上升沿） */
void flow_init(void)
{
    HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
}

/* 处理一次输入捕获：把两次上升沿之间的计数值换算成频率 Hz 和瞬时流量 L/M */
void flow_ic_capture(void)
{
    uint32_t fre_count1 = HAL_TIM_ReadCapturedValue(&htim16, TIM_CHANNEL_1);
    __HAL_TIM_SET_COUNTER(&htim16, 0);

    if (fre_count1 > 0)
    {
        /* TIM16 80MHz / 80(预分频) = 1MHz 计数时钟 */
        fre1 = 1000000UL / fre_count1;
        F_value = fre1 / 200.0f;

        /* 异常频段：f>8000 视为传感器异常 */
        if (fre1 > 8000)
        {
            fre1    = 8001;          /* 钳到刚好越过 8000 阈值，让 LD1 触发 */
            F_value = 40.0f;
        }
        if (fre1 < 800)
        {
            F_value = 0.0f;          /* 频率太低视为无流量 */
        }
    }
}

/* 100ms 一次的累计：把 F × 100ms 折算成 L，累到 Q_acc；满 1L 才让 Q_value +1 */
void flow_accumulate_q(void)
{
    /* F 单位 L/M = L/60s，100ms 内的累计增量 = F × (0.1/60) L */
    Q_acc += F_value * (0.1f / 60.0f);
    if (Q_acc >= 1.0f)
    {
        Q_value += (uint32_t)Q_acc;
        Q_acc   -= (float)((uint32_t)Q_acc);
        if (Q_value > 999999999UL) Q_value = 999999999UL;
    }
}
