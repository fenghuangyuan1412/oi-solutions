# compare_p1.py -- 题 1/2: r1.exe 与 perm_brute.exe 对拍 (n=1..6, 3000 组)
import random, subprocess, sys, os
D = os.path.dirname(os.path.abspath(__file__))
R1 = os.path.join(D, "r1.exe")
BR = os.path.join(D, "perm_brute.exe")
MOD = 998244353
random.seed(20260816)

def gen(n, mode):
    if mode == 0:      # 小数值 1..5
        v = lambda: random.randint(1, 5)
    elif mode == 1:    # 中等
        v = lambda: random.randint(1, 10**6)
    else:              # 接近 mod
        v = lambda: random.randint(1, MOD - 1)
    a = [[v() for _ in range(n)] for _ in range(n)]
    s = str(n) + " " + " ".join(str(x) for r in a for x in r)
    return s.encode()

def run(exe, data):
    p = subprocess.run([exe], input=data, stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE, shell=False)
    return p.stdout.decode().strip(), p.stderr.decode().strip()

total = 0
bad = 0
per_n = {}
for n in range(1, 7):
    k = 500
    okn = 0
    for t in range(k):
        data = gen(n, t % 3)
        o1, _ = run(R1, data)
        o2, _ = run(BR, data)
        total += 1
        if o1 != o2:
            bad += 1
            print("MISMATCH n=%d r1=%s brute=%s\n%s" % (n, o1, o2, data.decode()))
            if bad > 5:
                sys.exit(1)
        else:
            okn += 1
    per_n[n] = okn
print("per-n matched:", per_n)
print("total groups = %d, mismatch = %d" % (total, bad))
print("RESULT:", "ALL MATCH -> r1 computes permanent" if bad == 0 else "MISMATCH FOUND")
