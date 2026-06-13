#include "led.h"
#include "gpio.h"
#include "main.h"
#include "settings.h"

/* 74HC595 移位寄存器驱动
 * PD2 = 锁存信号
 * PC8~PC15 = 数据/选通，低 4 位对应 LD1~LD4
 * 低电平点亮 */
void led_show(uint8_t ucled)
{
    /* 先把 PC8~PC15 全部置 1（熄灭所有 LED） */
    HAL_GPIO_WritePin(GPIOC,
                      GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                      GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
                      GPIO_PIN_SET);

    /* 锁存一次，把"全灭"状态写入 74HC595 */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

    /* 把要点亮的 LED 拉低（按位左移到 PC8~PC15） */
    HAL_GPIO_WritePin(GPIOC, (uint16_t)ucled << 8, GPIO_PIN_RESET);

    /* 再次锁存，输出最终状态 */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

/* 每 50ms 一次的 LED 状态判定
 * LD1：f > 8000Hz 立即亮
 * LD2：F < FL 且 D > DH 持续 2s
 * LD3：F > FH 且 P < PL 持续 2s
 * LD4：tar_changed（用户在 OUTP 改过目标且已生效） */
void led_tick(void)
{
    static uint32_t led2_t = 0, led3_t = 0;
    uint8_t new_led = 0x00;

    /* LD1：流量传感器异常 */
    if (fre1 > 8000) new_led |= LD1_BIT;

    /* LD2：管路堵塞 */
    if ((F_value < FL_value) && ((int8_t)D_actual > DH_value))
    {
        if (led2_t < ALARM_HOLD_MS) led2_t += LED_SCAN_MS;
        if (led2_t >= ALARM_HOLD_MS) new_led |= LD2_BIT;
    }
    else
    {
        led2_t = 0;
    }

    /* LD3：管路泄漏 */
    if ((F_value > FH_value) && (P_value < PL_value))
    {
        if (led3_t < ALARM_HOLD_MS) led3_t += LED_SCAN_MS;
        if (led3_t >= ALARM_HOLD_MS) new_led |= LD3_BIT;
    }
    else
    {
        led3_t = 0;
    }

    /* LD4：动态调节指示 */
    if (tar_changed) new_led |= LD4_BIT;

    /* 状态变化才刷新 74HC595（避免 I/O 抖动） */
    if (new_led != led_state)
    {
        led_state = new_led;
        led_show(led_state);
    }
}
