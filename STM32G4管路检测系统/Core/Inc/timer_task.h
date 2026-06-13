#ifndef __TIMER_TASK_H
#define __TIMER_TASK_H

#include "settings.h"

/* 由 HAL_TIM_PeriodElapsedCallback 在 TIM3 触发时调用
 * 内部按 50ms / 100ms / 1000ms 三个周期把工作分派给各模块 */
void timer_task_dispatch(void);

#endif /* __TIMER_TASK_H */
