#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""随机数据生成器（仅验证用，非讲解代码）。

用法：
  python gen.py <seed>                 # 小规模：R,C<=6, 值<=9  —— 供暴力对拍
  python gen.py <seed> mid             # 中规模：R,C<=30, 值<=4000 —— 正解 vs DP 版互校
  python gen.py <seed> big             # 大规模：R=C=500, 值<=4000 —— 供计时

输出格式与洛谷题面一致：第一行 R C A B，之后 R 行每行 C 个整数。
"""
import random
import sys


def main() -> None:
    seed = int(sys.argv[1])
    mode = sys.argv[2] if len(sys.argv) > 2 else "small"
    rnd = random.Random(seed)
    if mode == "big":
        R = C = 500
        vmax = 4000
    elif mode == "mid":
        R = rnd.randint(1, 30)
        C = rnd.randint(1, 30)
        vmax = 4000
    else:
        R = rnd.randint(1, 6)
        C = rnd.randint(1, 6)
        vmax = 9
    A = rnd.randint(1, R)
    B = rnd.randint(1, C)
    out = [f"{R} {C} {A} {B}"]
    for _ in range(R):
        out.append(" ".join(str(rnd.randint(0, vmax)) for _ in range(C)))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
