#ifndef __FLOW_H
#define __FLOW_H

#include "settings.h"

/* 启动 TIM16 输入捕获（MX_TIM16_Init 之后调用一次） */
void flow_init(void);

/* 由 HAL_TIM_IC_CaptureCallback 调用：处理一次 TIM16 捕获值，算频率和瞬时流量 */
void flow_ic_capture(void);

/* 由 timer_task 每 Q_SAMPLE_MS 调用一次：把 F 累加到 Q_value（满 1L 才 +1） */
void flow_accumulate_q(void);

#endif /* __FLOW_H */
