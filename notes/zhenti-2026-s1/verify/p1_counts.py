# p1_counts.py -- 题 6(20 题复杂度) / 题 7(21 题 n=14) / 1ll 溢出阈值定位
import random, subprocess, os
D = os.path.dirname(os.path.abspath(__file__))
MOD = 998244353
def run(exe, data):
    p = subprocess.run([os.path.join(D, exe)], input=data.encode(),
                       stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return p.stdout.decode(), p.stderr.decode()
def inp(n, hi=5):
    random.seed(n * 991 + 7)
    a = [[random.randint(1, hi) for _ in range(n)] for _ in range(n)]
    return str(n) + " " + " ".join(str(x) for r in a for x in r)

print("### item6/7: if 条件为真次数（第 15 行）与循环体执行次数")
print("%3s %14s %14s %14s %10s %12s" % ("n", "if_true", "n*2^(n-1)", "if_checks", "n*2^n", "ratio"))
for n in list(range(10, 21)) + [13, 14, 15]:
    o, e = run("r1_count.exe", inp(n))
    kv = {}
    for line in e.splitlines():
        if "=" in line:
            k, v = line.split("=", 1)
            kv[k.strip()] = v.strip()
    it = int(kv["if_true  (line15 true)"]); ic = int(kv["if_checks (line15 eval)"])
    print("%3d %14d %14d %14d %10d %12.6f" %
          (n, it, n * (1 << (n - 1)), ic, n * (1 << n), it / (n * 2.0 ** n)))

print()
print("### item5b: 1ll* 溢出阈值精确定位（n=2，四个格子同为 v）")
for v in [1, 46340, 46341, 100000, 998244352]:
    m = [[v, v], [v, v]]
    s = "2 " + " ".join(str(x) for r in m for x in r)
    o1, _ = run("r1.exe", s)
    o2, _ = run("r1_noll.exe", s)
    exact = (2 * v * v) % MOD
    print("v=%-10d v^2=%-21d r1=%-12s r1_noll=%-14s exact=%-12s %s" %
          (v, v * v, o1, o2, exact, "SAME" if o2 == str(exact) else "DIFF"))
