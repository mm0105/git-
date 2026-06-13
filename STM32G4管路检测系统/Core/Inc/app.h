#ifndef __APP_H
#define __APP_H

/* 应用层入口：在 main() 之前完成硬件 + 各模块初始化，并启动 TIM3 */
void app_init(void);

/* 主循环：按键处理 + LCD 刷新，由 main() 的 while(1) 调用 */
void app_run(void);

#endif /* __APP_H */
