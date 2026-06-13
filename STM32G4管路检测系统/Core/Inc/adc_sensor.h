#ifndef __ADC_SENSOR_H
#define __ADC_SENSOR_H

#include "settings.h"

/* 启动 ADC2 校准（MX_ADC2_Init 之后调用一次） */
void adc_sensor_init(void);

/* 读取一次 ADC 并返回电压值（0~3.3V），同时刷新 Vin_Voltage / adc_value2 */
float adc_get_voltage(void);

/* 读取 ADC 并按 Voffset 线性映射为压力（0~10.0 Bar），同时刷新 P_value */
float adc_get_pressure(void);

#endif /* __ADC_SENSOR_H */
