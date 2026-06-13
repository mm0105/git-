#ifndef __KEY_H
#define __KEY_H

#include <stdint.h>
#include "settings.h"

/* 4 个按键：PB0=B1, PB1=B2, PB2=B3, PA0=B4 */
#define KEY_B1   0   /* 界面切换 */
#define KEY_B2   1   /* 模式/选项 */
#define KEY_B3   2   /* 加 / 长按=零点校准 */
#define KEY_B4   3   /* 减 / 长按=累计清零 */

struct keys
{
    uint8_t  key_now;          /* 当前电平 0=按下 1=松开 */
    uint8_t  key_judge;        /* 状态机：0=空闲 1=按下确认 2=按下中 */
    uint32_t key_press_tick;   /* 按下时的 HAL_GetTick() 值（ms） */
    uint8_t  key_duan_flag;    /* 短按事件 */
    uint8_t  key_long_flag;    /* 长按事件 */
    uint8_t  key_long_fired;   /* 本次按下长按已触发（避免重触） */
};

extern struct keys key[4];

/* 由 TIM3 中断调用：扫描 4 个按键 GPIO，状态机推进并置位短按/长按事件 */
void key_scan(void);

/* 由主循环调用：根据 Lcd_Now_Page 处理 4 个按键短按/长按事件 */
void key_handle(void);

#endif /* __KEY_H */
