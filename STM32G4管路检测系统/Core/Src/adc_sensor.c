#include "adc_sensor.h"
#include "adc.h"
#include "settings.h"

/* ADC2 初始化时调用一次：内部偏移校准 */
void adc_sensor_init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
}

/* 读取一次 ADC 并返回电压值（0~3.3V） */
float adc_get_voltage(void)
{
    HAL_ADC_Start(&hadc2);
    if (HAL_ADC_PollForConversion(&hadc2, 10) == HAL_OK)
    {
        adc_value2 = HAL_ADC_GetValue(&hadc2);
    }
    Vin_Voltage = (float)adc_value2 * 3.3f / 4096.0f;
    return Vin_Voltage;
}

/* 读取 ADC 并按 Voffset 线性映射为压力（0~10.0 Bar） */
float adc_get_pressure(void)
{
    float v = adc_get_voltage();
    if (v <= Voffset)
    {
        P_value = 0.0f;
    }
    else
    {
        float range = 3.3f - Voffset;
        if (range < 0.01f) range = 0.01f;       /* 防止除零 */
        P_value = (v - Voffset) * 10.0f / range;
        if (P_value > 10.0f) P_value = 10.0f;
    }
    return P_value;
}
