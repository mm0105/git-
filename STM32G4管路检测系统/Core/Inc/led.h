#ifndef __LED_H
#define __LED_H

#include "settings.h"

/* 4 个 LED 的位定义（低电平点亮，对应 74HC595 移位寄存器） */
#define LD1_BIT   0x01   /* 流量传感器异常 */
#define LD2_BIT   0x02   /* 管路堵塞 */
#define LD3_BIT   0x04   /* 管路泄漏 */
#define LD4_BIT   0x08   /* 动态调节指示 */

/* 由 led.c 内置 50ms 扫描周期：LD2/LD3 持续 2s 才触发
 * 由 timer_task 每 LED_SCAN_MS 调用一次即可 */
void led_tick(void);

/* 直接驱动 74HC595：低 4 位有效
 * bit0=LD1, bit1=LD2, bit2=LD3, bit3=LD4
 * 0=亮, 1=灭 */
void led_show(uint8_t ucled);

#endif /* __LED_H */
