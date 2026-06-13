#include "key.h"
#include "main.h"
#include "gpio.h"
#include "settings.h"

struct keys key[4] = {0};

/* ============ TIM3 中断里调用：按键状态机扫描 ============ */
void key_scan(void)
{
    uint32_t now = HAL_GetTick();

    /* 读 4 个按键当前电平 */
    key[0].key_now = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);   /* B1 */
    key[1].key_now = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);   /* B2 */
    key[2].key_now = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);   /* B3 */
    key[3].key_now = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);   /* B4 */

    for (uint8_t i = 0; i < 4; i++)
    {
        switch (key[i].key_judge)
        {
            case 0:                                       /* 空闲 */
                if (key[i].key_now == 0)                  /* 检测到按下 */
                {
                    key[i].key_press_tick = now;
                    key[i].key_long_fired  = 0;
                    key[i].key_judge = 1;
                }
                break;
            case 1:                                       /* 按下确认（第二帧防抖） */
                if (key[i].key_now == 0) key[i].key_judge = 2;
                else                     key[i].key_judge = 0;
                break;
            case 2:                                       /* 持续按下中 */
                if (key[i].key_now == 1)                  /* 松手 */
                {
                    if (!key[i].key_long_fired)           /* 没触发过长按，按本次作为短按 */
                        key[i].key_duan_flag = 1;
                    key[i].key_judge = 0;
                }
                else                                      /* 仍按下：到 2s 立即触发长按 */
                {
                    if (!key[i].key_long_fired &&
                        (now - key[i].key_press_tick) >= KEY_LONG_MS)
                    {
                        key[i].key_long_flag  = 1;
                        key[i].key_long_fired = 1;
                    }
                }
                break;
            default:
                key[i].key_judge = 0;
                break;
        }
    }
}

/* ============ 主循环里调用：处理短按/长按事件 ============ */
void key_handle(void)
{
    /* ---------- B1：界面切换 ---------- */
    if (key[KEY_B1].key_duan_flag == 1)
    {
        key[KEY_B1].key_duan_flag = 0;

        /* 离开 OUTP 时：先把选项复位；如果本次在 OUTP 里改过，再触发 LD4 */
        if (Lcd_Now_Page == PAGE_OUTPUT)
        {
            B2_flag2 = 1;
            if (outp_dirty) { tar_changed = 1; outp_dirty = 0; }
        }
        if (Lcd_Now_Page == PAGE_PARA) B2_flag3 = 1;

        Lcd_Now_Page++;
        if (Lcd_Now_Page > PAGE_PARA) Lcd_Now_Page = PAGE_MAIN;
        /* 进入 OUTP 时：清掉"已修改"标志，避免下次退出误触发 LD4 */
        if (Lcd_Now_Page == PAGE_OUTPUT) outp_dirty = 0;
        Lcd_Refresh_Flag = 1;
    }

    /* ---------- B2：功能切换（模式/选项）---------- */
    if (key[KEY_B2].key_duan_flag == 1)
    {
        key[KEY_B2].key_duan_flag = 0;

        if (Lcd_Now_Page == PAGE_MAIN)
        {
            B2_flag1 = !B2_flag1;     /* MAN <-> AUTO */
        }
        else if (Lcd_Now_Page == PAGE_OUTPUT)
        {
            B2_flag2 = !B2_flag2;     /* TarD <-> TarP */
        }
        else if (Lcd_Now_Page == PAGE_PARA)
        {
            B2_flag3++;
            if (B2_flag3 > 4) B2_flag3 = 1;   /* FH -> FL -> PL -> DH -> FH */
        }
        Lcd_Refresh_Flag = 1;
    }

    /* ---------- B3：加 ---------- */
    if (key[KEY_B3].key_duan_flag == 1)
    {
        key[KEY_B3].key_duan_flag = 0;

        if (Lcd_Now_Page == PAGE_OUTPUT)
        {
            if (B2_flag2 == 1)       /* TarD */
            {
                TarD_value += 5;
                if (TarD_value > 95) TarD_value = 95;
                outp_dirty = 1;
            }
            else                     /* TarP */
            {
                TarP_value += 0.5f;
                if (TarP_value > 9.5f) TarP_value = 9.5f;
                outp_dirty = 1;
            }
        }
        else if (Lcd_Now_Page == PAGE_PARA)
        {
            switch (B2_flag3)
            {
                case 1: FH_value += 1.0f; if (FH_value > 40.0f) FH_value = 40.0f; break;
                case 2: FL_value += 1.0f; if (FL_value > 40.0f) FL_value = 40.0f; break;
                case 3: PL_value += 0.5f; if (PL_value > 9.5f)  PL_value = 9.5f;  break;
                case 4: DH_value += 10;   if (DH_value > 95)   DH_value = 95;    break;
                default: break;
            }
        }
        Lcd_Refresh_Flag = 1;
    }

    /* ---------- B4：减 ---------- */
    if (key[KEY_B4].key_duan_flag == 1)
    {
        key[KEY_B4].key_duan_flag = 0;

        if (Lcd_Now_Page == PAGE_OUTPUT)
        {
            if (B2_flag2 == 1)       /* TarD */
            {
                if (TarD_value >= 5) TarD_value -= 5;
                else                 TarD_value = 5;
                outp_dirty = 1;
            }
            else                     /* TarP */
            {
                TarP_value -= 0.5f;
                if (TarP_value < 1.0f) TarP_value = 1.0f;
                outp_dirty = 1;
            }
        }
        else if (Lcd_Now_Page == PAGE_PARA)
        {
            switch (B2_flag3)
            {
                case 1: if (FH_value > 4.0f) FH_value -= 1.0f; else FH_value = 4.0f; break;
                case 2: if (FL_value > 4.0f) FL_value -= 1.0f; else FL_value = 4.0f; break;
                case 3: if (PL_value > 1.0f) PL_value -= 0.5f; else PL_value = 1.0f; break;
                case 4: if (DH_value > 5)    DH_value -= 10;   else DH_value = 5;    break;
                default: break;
            }
        }
        Lcd_Refresh_Flag = 1;
    }

    /* ---------- B3 长按 2s：监控界面下零点校准 ---------- */
    if (key[KEY_B3].key_long_flag == 1)
    {
        key[KEY_B3].key_long_flag = 0;
        if (Lcd_Now_Page == PAGE_MAIN)
        {
            Voffset = Vin_Voltage;   /* 把当前电压存为新 Voffset */
        }
        Lcd_Refresh_Flag = 1;
    }

    /* ---------- B4 长按 2s：监控界面下累计流量清零 ---------- */
    if (key[KEY_B4].key_long_flag == 1)
    {
        key[KEY_B4].key_long_flag = 0;
        if (Lcd_Now_Page == PAGE_MAIN)
        {
            Q_value = 0;
            Q_acc   = 0.0f;
        }
        Lcd_Refresh_Flag = 1;
    }
}
