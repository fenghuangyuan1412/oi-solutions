# fill2_sample.py -- 给讲义用的一个可手算小样例（含暴力核对）
import os, sys, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
D = os.path.dirname(os.path.abspath(__file__))

# 主链 1-2-3-4-5，边长 a1..a4 = 2 3 1 4
# 叶边编号：1(挂1,长5) 2(挂1,长7) 3(挂2,长2) 4(挂4,长6) 5(挂5,长3)
lines = ["5 5 7", "2 3 1 4",
         "2 5 7",   # 顶点1 的叶边：编号1(5), 编号2(7)
         "1 2",     # 顶点2 的叶边：编号3(2)
         "0",       # 顶点3 无叶边
         "1 6",     # 顶点4 的叶边：编号4(6)
         "1 3",     # 顶点5 的叶边：编号5(3)
         "3 1 5",   # 询问整棵树
         "3 2 3",   # 询问顶点 2,3 及其叶子
         "3 4 4",   # 只问顶点 4 的星
         "2 1 20",  # 把叶边 1 改成 20
         "3 1 5",
         "1 2 10",  # 把主链边 a2 改成 10
         "3 1 5"]
inp = "\n".join(lines) + "\n"
open(os.path.join(D, "f2_sample.txt"), "w").write(inp)

a = [0, 2, 3, 1, 4]
bel = {1: 1, 2: 1, 3: 2, 4: 4, 5: 5}
lw = {1: 5, 2: 7, 3: 2, 4: 6, 5: 3}


def dist(i, j):
    s = 0
    if i > j:
        i, j = j, i
    for t in range(i, j):
        s += a[t]
    return s


def diam(x, y):
    pts = []
    for v in range(x, y + 1):
        pts.append(("V%d" % v, 0, v))
    for lid, bv in bel.items():
        if x <= bv <= y:
            pts.append(("L%d" % lid, lw[lid], bv))
    best = 0
    for i in range(len(pts)):
        for j in range(i + 1, len(pts)):
            d = pts[i][1] + pts[j][1] + dist(pts[i][2], pts[j][2])
            best = max(best, d)
    return best


out = []
for ln in lines[7:]:
    p = ln.split()
    o, x, y = int(p[0]), int(p[1]), int(p[2])
    if o == 1:
        a[x] = y
    elif o == 2:
        lw[x] = y
    else:
        out.append(str(diam(x, y)))
print("=== 输入 ===")
print(inp)
print("=== 暴力（所有点对最大距离）===")
print("\n".join(out))

exe = os.path.join(D, "fill2.exe")
if not os.path.exists(exe):
    s = open(os.path.join(D, "fill2_tpl.cpp"), encoding="utf-8").read()
    from fill2_run import OPTS, ANS
    for kk in ("B1", "B2", "B3", "B4", "B5"):
        s = s.replace("@@%s@@" % kk, OPTS[kk][ANS[kk]])
    open(os.path.join(D, "fill2.cpp"), "w", encoding="utf-8").write(s)
    subprocess.run(["g++", "-static", "-O2", "-std=c++14", os.path.join(D, "fill2.cpp"), "-o", exe], check=True)
r = subprocess.run([exe], input=inp.encode(), stdout=subprocess.PIPE)
print("=== 官方答案程序 ===")
print(r.stdout.decode())
ex = out
gt = r.stdout.decode().split()
print("=== 一致？", "是" if ex == gt else "否", "===")
