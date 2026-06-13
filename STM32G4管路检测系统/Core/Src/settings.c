#include "settings.h"

/* LCD 界面与刷新 */
uint8_t  Lcd_Now_Page        = PAGE_MAIN;
uint8_t  Lcd_Refresh_Flag    = 1;

/* ADC 与压力 */
uint32_t adc_value2          = 0;
float    Vin_Voltage         = 0.0f;
float    Voffset             = 0.0f;
float    P_value             = 0.0f;

/* 流量与累计 */
uint32_t fre1                = 0;
float    F_value             = 0.0f;
uint32_t Q_value             = 0;
float    Q_acc               = 0.0f;

/* PWM */
uint8_t  D_actual            = 5;
uint8_t  TarD_value          = 5;
float    TarP_value          = 5.0f;

/* 报警阈值 */
float    FH_value            = 20.0f;
float    FL_value            = 10.0f;
float    PL_value            = 1.0f;
int8_t   DH_value            = 65;

/* 模式与选项 */
uint8_t  B2_flag1            = 1;
uint8_t  B2_flag2            = 1;
uint8_t  B2_flag3            = 1;

/* LD4 标志 */
uint8_t  tar_changed         = 0;
uint8_t  outp_dirty          = 0;

/* LED 总线 */
uint8_t  led_state           = 0x00;
