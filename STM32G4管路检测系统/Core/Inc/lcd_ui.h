#ifndef __LCD_UI_H
#define __LCD_UI_H

#include "settings.h"

/* 初始化 LCD（清屏 + 黑白配色） */
void lcd_ui_init(void);

/* 三页静态刷新：标题 + 选项；
 * 由 Lcd_Refresh 在 Lcd_Refresh_Flag 置位（页面切换）时调用 */
void lcd_Show_Page1(void);   /* MAIN 监控 */
void lcd_Show_Page2(void);   /* OUTP 输出配置 */
void lcd_Show_Page3(void);   /* PARA 运行参数 */

/* 周期性刷新：页面切换时整页重绘，监控页每 100ms 刷数据行避免闪烁 */
void Lcd_Refresh(void);

#endif /* __LCD_UI_H */
