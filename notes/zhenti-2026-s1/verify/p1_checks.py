# p1_checks.py -- 题 3(16 越界) / 题 4(17 输出 0 构造) / 题 5(18 去 1ll 反例) / 题 6,7 计数
import random, subprocess, os
D = os.path.dirname(os.path.abspath(__file__))
MOD = 998244353
def run(exe, data):
    p = subprocess.run([os.path.join(D, exe)], input=data.encode() if isinstance(data, str) else data,
                       stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return p.stdout.decode(), p.stderr.decode()
def inp(n, a):
    return str(n) + " " + " ".join(str(x) for r in a for x in r)

print("### item3: n=20 边界下标（r1_count.exe）")
random.seed(7)
a = [[random.randint(1, MOD - 1) for _ in range(20)] for _ in range(20)]
o, e = run("r1_count.exe", inp(20, a))
print(e)
print("output =", o)

print("### item3b: n=20 直接跑原版 r1.exe（观察是否崩溃 / 退出码）")
p = subprocess.run([os.path.join(D, "r1.exe")], input=inp(20, a).encode(),
                   stdout=subprocess.PIPE, stderr=subprocess.PIPE)
print("exitcode =", p.returncode, "output =", p.stdout.decode()[:200])

print("### item4: 17 题 构造输出为 0")
for tag, n, m in [
    ("n=2 [[1,1],[MOD-1,1]]", 2, [[1, 1], [MOD - 1, 1]]),
    ("n=2 [[1,119],[8388608,1]]", 2, [[1, 119], [8388608, 1]]),
    ("n=2 [[1,2],[(MOD-1)//2,1]] (需 MOD 奇)", 2, [[1, 2], [(MOD - 1) // 2, 1]]),
    ("n=1 唯一格子 a=MOD-1", 1, [[MOD - 1]]),
]:
    o, e = run("r1.exe", inp(n, m))
    print("%-42s -> %s" % (tag, o))
# 精确值验证（任意精度）
print("exact perm [[1,1],[MOD-1,1]] =", 1 * 1 + 1 * (MOD - 1), "mod =", (1 + MOD - 1) % MOD)
print("exact perm [[1,119],[2^23,1]] =", 1 * 1 + 119 * 8388608, "mod =", (1 + 119 * 8388608) % MOD)

print("### item5: 18 题 去掉 1ll* 的反例搜索（n=2,3）")
cands = []
tests = [("n=2 all=100000", 2, [[100000, 100000], [100000, 100000]])]
random.seed(1)
for t in range(400):
    n = random.choice([2, 2, 3])
    hi = random.choice([10**3, 10**5, 10**7, MOD - 1])
    m = [[random.randint(1, hi) for _ in range(n)] for _ in range(n)]
    tests.append(("rand n=%d hi=%d" % (n, hi), n, m))
diff = 0
for tag, n, m in tests:
    o1, _ = run("r1.exe", inp(n, m))
    o2, _ = run("r1_noll.exe", inp(n, m))
    # 真值
    def perm(mm):
        import itertools
        k = len(mm)
        s = 0
        for p in itertools.permutations(range(k)):
            t = 1
            for i in range(k):
                t *= mm[i][p[i]]
            s += t
        return s
    ex = perm(m) % MOD
    if str(ex) != o2:
        diff += 1
        if len(cands) < 6:
            cands.append((tag, n, m, o1, o2, ex))
print("差异组数 = %d / %d" % (diff, len(tests)))
for tag, n, m, o1, o2, ex in cands:
    print("case[%s] matrix=%s r1=%s r1_noll=%s exact=%d" % (tag, m, o1, o2, ex))
