# fill2_dbg.py -- 打印单组数据的输入/暴力输出/官方程序输出，定位不一致
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from fill2_run import gen, run, D

seed = int(sys.argv[1]) if len(sys.argv) > 1 else 4
inp, expect = gen(seed)
exe = os.path.join(D, "fill2_B1D.exe")
rc, out, err = run(exe, inp)
print("=== 输入 ===")
print(inp)
print("=== 暴力 ===")
print(expect)
print("=== 程序(exit=%d) ===" % rc)
print(out)
ex = expect.split(); got = out.split()
print("=== 差异位置 ===")
for i in range(max(len(ex), len(got))):
    a = ex[i] if i < len(ex) else "-"
    b = got[i] if i < len(got) else "-"
    if a != b:
        print("  第%d个询问: 暴力=%s 程序=%s" % (i + 1, a, b))
