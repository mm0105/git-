#include "app.h"
#include "led.h"
#include "lcd_ui.h"
#include "pwm.h"
#include "flow.h"
#include "adc_sensor.h"
#include "key.h"
#include "settings.h"
#include "tim.h"
#include "main.h"

/* 硬件 + 各模块初始化 */
void app_init(void)
{
    /* 关闭所有 LED（74HC595 默认上电态由锁存控制） */
    led_show(0x00);

    /* LCD 黑底白字 */
    lcd_ui_init();

    /* ADC 内部偏移校准 */
    adc_sensor_init();

    /* 启动 TIM2_CH2 PWM，初始 5% */
    pwm_init();

    /* 启动 TIM16 输入捕获（CH1 上升沿） */
    flow_init();

    /* 启动 TIM3 1ms 周期中断（按键扫描 / 任务调度） */
    HAL_TIM_Base_Start_IT(&htim3);
}

/* 主循环：按键 + LCD */
void app_run(void)
{
    key_handle();
    Lcd_Refresh();
}
