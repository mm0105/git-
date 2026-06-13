#ifndef __SETTINGS_H
#define __SETTINGS_H

#include <stdint.h>

/* LCD 界面编号 */
#define PAGE_MAIN      1
#define PAGE_OUTPUT    2
#define PAGE_PARA      3

/* 时间阈值（ms 全部基于 HAL_GetTick 1ms 基准） */
#define KEY_LONG_MS    2000    /* 长按：持时 >= 2s */
#define PWM_STEP_MS    1000    /* PWM 1%/秒 */
#define Q_SAMPLE_MS    100     /* 流量累计采样 100ms */
#define LED_SCAN_MS    50      /* LED 状态扫描 50ms */
#define ALARM_HOLD_MS  2000    /* LD2/LD3 报警持续 2s 触发 */

/* 全局显示状态 */
extern uint8_t  Lcd_Now_Page;
extern uint8_t  Lcd_Refresh_Flag;

/* ADC 与压力 */
extern uint32_t adc_value2;
extern float    Vin_Voltage;
extern float    Voffset;        /* 零点偏置电压（同时显示在 LCD 的 V 上） */
extern float    P_value;        /* 实时压力 Bar */

/* 流量与累计 */
extern uint32_t fre1;           /* 脉冲频率 Hz */
extern float    F_value;        /* 瞬时流量 L/M */
extern uint32_t Q_value;        /* 累计流量 L（仅满 1L 才更新） */
extern float    Q_acc;          /* 累计小数缓存 */

/* PWM */
extern uint8_t  D_actual;       /* 实际 PWM 占空比（写到 CCR 的值） */
extern uint8_t  TarD_value;     /* 手动模式目标占空比 5~95 */
extern float    TarP_value;     /* 自动模式目标压力 1.0~9.5 BAR */

/* 报警阈值（PARA 页可调） */
extern float    FH_value;       /* 流量上限 L/M */
extern float    FL_value;       /* 流量下限 L/M */
extern float    PL_value;       /* 压力下限 BAR */
extern int8_t   DH_value;       /* 占空比上限 % */

/* 模式与页面选项 */
extern uint8_t  B2_flag1;       /* 1=MAN, 0=AUTO */
extern uint8_t  B2_flag2;       /* OUTP 页：1=TarD, 0=TarP */
extern uint8_t  B2_flag3;       /* PARA 页：1=FH, 2=FL, 3=PL, 4=DH */

/* LD4 动态指示 */
extern uint8_t  tar_changed;    /* 目标值生效中（用于 LD4） */
extern uint8_t  outp_dirty;     /* OUTP 页内被改过（退出时再触发 LD4） */

/* LED 总线状态 */
extern uint8_t  led_state;      /* 当前 74HC595 输出（低 4 位有效） */

#endif /* __SETTINGS_H */
