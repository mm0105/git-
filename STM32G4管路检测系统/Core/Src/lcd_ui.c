#include "lcd_ui.h"
#include "lcd.h"
#include "main.h"
#include "settings.h"
#include <stdio.h>

/* 初始化 LCD 颜色 + 清屏 */
void lcd_ui_init(void)
{
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
}

/* 监控界面（标题 + 各数据项） */
void lcd_Show_Page1(void)
{
    char text[30];
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);

    sprintf(text, "       MAIN         ");
    LCD_DisplayStringLine(Line1, (uint8_t *)text);

    sprintf(text, "                    ");
    LCD_DisplayStringLine(Line2, (uint8_t *)text);

    if (B2_flag1 == 1)
        sprintf(text, "   M=MAN            ");
    else
        sprintf(text, "   M=AUTO           ");
    LCD_DisplayStringLine(Line3, (uint8_t *)text);

    sprintf(text, "   P=%.1fBAR         ", P_value);
    LCD_DisplayStringLine(Line4, (uint8_t *)text);

    sprintf(text, "   F=%.1fL/M         ", F_value);
    LCD_DisplayStringLine(Line5, (uint8_t *)text);

    sprintf(text, "   Q=%dL            ", Q_value);
    LCD_DisplayStringLine(Line6, (uint8_t *)text);

    sprintf(text, "   D=%d%%            ", D_actual);
    LCD_DisplayStringLine(Line7, (uint8_t *)text);

    sprintf(text, "   V=%.1fV           ", Voffset);
    LCD_DisplayStringLine(Line8, (uint8_t *)text);
}

/* 输出配置界面 */
void lcd_Show_Page2(void)
{
    char text[30];
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);

    sprintf(text, "       OUTP         ");
    LCD_DisplayStringLine(Line1, (uint8_t *)text);

    sprintf(text, "                    ");
    LCD_DisplayStringLine(Line2, (uint8_t *)text);

    sprintf(text, "   TarD=%d%%          ", TarD_value);
    LCD_DisplayStringLine(Line3, (uint8_t *)text);

    sprintf(text, "   TarP=%.1fBAR       ", TarP_value);
    LCD_DisplayStringLine(Line4, (uint8_t *)text);

    sprintf(text, "                    ");
    LCD_DisplayStringLine(Line5, (uint8_t *)text);
    LCD_DisplayStringLine(Line6, (uint8_t *)text);
    LCD_DisplayStringLine(Line7, (uint8_t *)text);
    LCD_DisplayStringLine(Line8, (uint8_t *)text);
}

/* 运行参数界面 */
void lcd_Show_Page3(void)
{
    char text[30];
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);

    sprintf(text, "       PARA         ");
    LCD_DisplayStringLine(Line1, (uint8_t *)text);

    sprintf(text, "                    ");
    LCD_DisplayStringLine(Line2, (uint8_t *)text);

    sprintf(text, "   FH=%.1fL/M         ", FH_value);
    LCD_DisplayStringLine(Line3, (uint8_t *)text);

    sprintf(text, "   FL=%.1fL/M         ", FL_value);
    LCD_DisplayStringLine(Line4, (uint8_t *)text);

    sprintf(text, "   PL=%.1fBAR         ", PL_value);
    LCD_DisplayStringLine(Line5, (uint8_t *)text);

    sprintf(text, "   DH=%d%%            ", DH_value);
    LCD_DisplayStringLine(Line6, (uint8_t *)text);

    sprintf(text, "                    ");
    LCD_DisplayStringLine(Line7, (uint8_t *)text);
    LCD_DisplayStringLine(Line8, (uint8_t *)text);
}

/* 整页重绘（页面切换时）+ 监控页 100ms 数据刷新 */
void Lcd_Refresh(void)
{
    static uint32_t Last_Time = 0;
    uint32_t Now_Time = HAL_GetTick();

    /* 1) 页面切换：强制完整重绘 */
    if (Lcd_Refresh_Flag == 1)
    {
        Lcd_Refresh_Flag = 0;
        LCD_Clear(Black);
        LCD_SetBackColor(Black);
        LCD_SetTextColor(White);

        switch (Lcd_Now_Page)
        {
            case PAGE_MAIN:   lcd_Show_Page1(); break;
            case PAGE_OUTPUT: lcd_Show_Page2(); break;
            case PAGE_PARA:   lcd_Show_Page3(); break;
            default: break;
        }
        Last_Time = Now_Time;
        return;
    }

    /* 2) 监控界面每 100ms 刷数据行（避免标题闪烁） */
    if (Lcd_Now_Page == PAGE_MAIN && (Now_Time - Last_Time > 100))
    {
        Last_Time = Now_Time;

        char text[30];
        LCD_SetBackColor(Black);
        LCD_SetTextColor(White);

        if (B2_flag1 == 1)
            sprintf(text, "   M=MAN            ");
        else
            sprintf(text, "   M=AUTO           ");
        LCD_DisplayStringLine(Line3, (uint8_t *)text);

        sprintf(text, "   P=%.1fBAR         ", P_value);
        LCD_DisplayStringLine(Line4, (uint8_t *)text);

        sprintf(text, "   F=%.1fL/M         ", F_value);
        LCD_DisplayStringLine(Line5, (uint8_t *)text);

        sprintf(text, "   Q=%dL            ", Q_value);
        LCD_DisplayStringLine(Line6, (uint8_t *)text);

        sprintf(text, "   D=%d%%            ", D_actual);
        LCD_DisplayStringLine(Line7, (uint8_t *)text);

        sprintf(text, "   V=%.1fV           ", Voffset);
        LCD_DisplayStringLine(Line8, (uint8_t *)text);
    }
}
