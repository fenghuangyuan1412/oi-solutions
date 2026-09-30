# p2_diff23.py -- 题 23 实测：把第 11 行改成 i%3<2 后，整份程序输出是否逐字不变
import subprocess, os, random
D = os.path.dirname(os.path.abspath(__file__))
def run(exe, s):
    p = subprocess.run([os.path.join(D, exe)], input=s.encode(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if p.returncode != 0:
        return "EXIT%d:%s" % (p.returncode, p.stderr.decode()[:120])
    return p.stdout.decode()
ns = list(range(1, 400)) + [987, 1597, 2584, 4181, 100000, 999999, 1000001, 12345678,
                            10**12, 10**17, 10**18 - 1, 999999999999999999, 233, 232, 234]
random.seed(3)
ns += [random.randint(1, 10**18) for _ in range(60)]
bad = 0
for n in ns:
    for p in (0, 1):
        s = "%d %d" % (n, p)
        a, b = run("r2.exe", s), run("r2_alt23.exe", s)
        if a != b:
            bad += 1
            print("DIFF", s, repr(a), repr(b))
print("checked %d inputs, output differences = %d" % (len(ns) * 2, bad))
