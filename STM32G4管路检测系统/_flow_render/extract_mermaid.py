#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
把 程序流程图.md 里的 mermaid 块抽成单独的 .mmd 文件，方便 mmdc 渲染。
"""
import re
import os
from pathlib import Path

ROOT = Path(r"d:\tools\stm32workspace\stm32G4\完成分割\17403250")
SRC = ROOT / "程序流程图.md"
OUT = ROOT / "_flow_render"
OUT.mkdir(exist_ok=True)

text = SRC.read_text(encoding="utf-8")

# 找所有 ```mermaid ... ``` 块 + 块前面那行的 ## 标题
# 必须 ## 0./## 1. 数字开头才认
pattern = re.compile(r"^(## \d+\..+?)\n+```mermaid\n(.*?)```", re.M | re.S)
matches = pattern.findall(text)

print(f"找到 {len(matches)} 个 mermaid 块")
for title, body in matches:
    # 标题转文件名: "## 0. 主程序流程图 (main.c)" -> "00_主程序流程图"
    title_clean = title.lstrip("# ").strip()
    safe = re.sub(r"[\\/:*?\"<>|()（）]", "_", title_clean)
    safe = re.sub(r"\s+", "_", safe)
    safe = safe[:60]
    safe = re.sub(r"_+", "_", safe).strip("_")
    fp = OUT / f"{safe}.mmd"
    # 去掉 %%{init}%% 头（draw.io 不需要、mmdc 也兼容但不必要）
    body_clean = re.sub(r"^%%\{init:.*?\}%%\s*\n", "", body, flags=re.M)
    # 去掉 linkStyle 曲线指令（mmdc 也兼容，但生成的 PNG 已经是直线无需）
    body_clean = re.sub(r"^\s*linkStyle\s+default.*$\n?", "", body_clean, flags=re.M)
    fp.write_text(body_clean.strip() + "\n", encoding="utf-8")
    print(f"  {fp.name}")

print(f"\n输出目录: {OUT}")
