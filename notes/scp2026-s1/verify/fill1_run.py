# fill1_run.py -- 完善程序(1)：按官方答案补全 + 逐空把 4 个选项分别填进去实测
# 报告：编译失败 / 运行崩溃(退出码) / 输出与朴素 qpow 不一致(首个不同位置) / 完全一致(+耗时)
import subprocess, os, sys, time, re
D = os.path.dirname(os.path.abspath(__file__))
TPL = os.path.join(D, "fill1_tpl.cpp")
N_TEST = int(sys.argv[1]) if len(sys.argv) > 1 else 3000
N_TIME = int(sys.argv[2]) if len(sys.argv) > 2 else 10000000

OPTS = {
 "B1": {  # 34 官方 A
   "A": "a = 1ull * a * a % mod;",
   "B": "a = 1ull * a * b % mod;",
   "C": "res = 1ull * a * a % mod;",
   "D": "a = 1ull * res * res % mod;",
 },
 "B2": {  # 35 官方 D
   "A": "bsgs2[i][j] = 1ull * bsgs2[i][j - 1] * qpow(i, B) % mod;",
   "B": "bsgs2[i][j] = 1ull * bsgs1[i][j - 1] * bsgs2[i][B - 1] % mod;",
   "C": "bsgs2[i][j] = 1ull * bsgs1[i][j - 1] * qpow(f[i], B) % mod;",
   "D": "bsgs2[i][j] = 1ull * bsgs2[i][j - 1] * bsgs1[i][B] % mod;",
 },
 "B3": {  # 36 官方 B
   "A": "for(int ex = 1; ex <= now; ex++)",
   "B": "for(int ex = gap + 1; ex <= now; ex++)",
   "C": "for(int ex = 1; ex <= now; ex += B)",
   "D": "for(int ex = gap + 1; ex <= now; ex += B)",
 },
 "B4": {  # 37 官方 B
   "A": ("f[pr[j] * i] = 1ull * bsgs2[pr[j] * i][i % B] * bsgs1[pr[j] * i][i / B] % mod * cur % mod;"),
   "B": ("f[pr[j] * i] = 1ull * bsgs1[pr[j]][i % B] * bsgs2[pr[j]][i / B] % mod * cur % mod;"),
   "C": ("f[pr[j] * i] = 1ull * bsgs2[pr[j]][i % B] * bsgs1[pr[j]][i / B] % mod * cur % mod;"),
   "D": ("f[pr[j] * i] = 1ull * bsgs1[pr[j] * i][i % B] * bsgs2[pr[j] * i][i / B] % mod * cur % mod;"),
 },
 "B5": {  # 38 官方 C
   "A": "pr[j] > i",
   "B": "!(pr[j] % i)",
   "C": "!(i % pr[j])",
   "D": "i > pr[j]",
 },
}
ANS = {"B1": "A", "B2": "D", "B3": "B", "B4": "B", "B5": "C"}   # 官方答案
QNO = {"B1": 34, "B2": 35, "B3": 36, "B4": 37, "B5": 38}

tpl = open(TPL, encoding="utf-8").read()

def make(blanks):
    s = tpl
    for k, v in blanks.items():
        s = s.replace("@@%s@@" % k, v)
    return s

def write_compile(src, base):
    cpp = os.path.join(D, base + ".cpp")
    exe = os.path.join(D, base + ".exe")
    open(cpp, "w", encoding="utf-8").write(src)
    r = subprocess.run(["g++", "-static", "-O2", "-std=c++14", cpp, "-o", exe],
                       capture_output=True, text=True)
    if r.returncode != 0:
        return None, r.stderr
    return exe, ""

def run_exe(exe, n):
    t0 = time.time()
    r = subprocess.run([exe, str(n)] if False else [exe], input="%d\n" % n,
                       capture_output=True, text=True)
    dt = time.time() - t0
    return r.stdout.strip(), r.returncode, dt

# 朴素答案
brute_exe, err = write_compile(open(os.path.join(D, "fill1_brute.cpp"), encoding="utf-8").read(), "fill1_brute")
assert brute_exe, err
exp, rc, _ = run_exe(brute_exe, N_TEST)
expected = exp.split()
print("朴素答案 (n=%d) 前 12 项: %s" % (N_TEST, " ".join(expected[:12])))
print("朴素答案 第 1000 项 =", expected[999], " 第 %d 项 = %s" % (N_TEST, expected[-1]))
print()

# 官方答案完整版
full, err = write_compile(make({k: OPTS[k][ANS[k]] for k in OPTS}), "fill1")
assert full, err
got, rc, dt = run_exe(full, N_TEST)
same = got.split() == expected
print("=== 官方答案 (34A 35D 36B 37B 38C) ===")
print("n=%d 与朴素 qpow 完全一致: %s   (运行 %.2fs)" % (N_TEST, "YES" if same else "NO", dt))
if not same:
    g = got.split()
    k = next(i for i in range(min(len(g), len(expected))) if g[i] != expected[i])
    print("   首个不同: i=%d 程序=%s 期望=%s" % (k + 1, g[k], expected[k]))
gx, rcx, dtx = run_exe(full, N_TIME)
print("n=%d 单跑耗时 %.2fs (exit=%d)" % (N_TIME, dtx, rcx))
print()

print("=== 逐空替换（每空独立，其余保持官方答案） ===")
print("%4s %3s %8s %10s %s" % ("题号", "选项", "编译", "运行", "结论"))
summary = {}
for blank in ["B1", "B2", "B3", "B4", "B5"]:
    for opt in "ABCD":
        name = "fill1_v_%s%s" % (blank, opt)
        src = make({**{k: OPTS[k][ANS[k]] for k in OPTS}, blank: OPTS[blank][opt]})
        exe, cerr = write_compile(src, name)
        if exe is None:
            print("%4d %3s %8s %10s %s" % (QNO[blank], opt, "FAIL", "-", "编译错误: " + cerr.strip().splitlines()[-1][:90]))
            summary[(blank, opt)] = "COMPILE_ERROR"
            continue
        got, rc, dt = run_exe(exe, N_TEST)
        g = got.split()
        tag = "官方答案" if opt == ANS[blank] else ""
        if rc != 0:
            res = "运行崩溃 exit=%d (0x%x)" % (rc, rc & 0xffffffff)
        elif len(g) != len(expected):
            res = "输出长度不对: %d 项 vs %d 项" % (len(g), len(expected))
        elif g == expected:
            res = "输出完全正确 (%.2fs)" % dt
        else:
            k = next(i for i in range(len(expected)) if g[i] != expected[i])
            nz = sum(1 for a, b in zip(g, expected) if a != b)
            res = "答案错误: 首个不同 i=%d 得 %s 应为 %s；共 %d/%d 项错" % (k + 1, g[k], expected[k], nz, len(expected))
        print("%4d %3s %8s %8.2fs %s %s" % (QNO[blank], opt, "ok", dt, res, tag))
        summary[(blank, opt)] = res
print()
print("=== 对'输出也正确'的选项，单独看 n=1e7 的耗时 ===")
for blank in ["B1", "B2", "B3", "B4", "B5"]:
    for opt in "ABCD":
        if summary[(blank, opt)].startswith("输出完全正确"):
            exe = os.path.join(D, "fill1_v_%s%s.exe" % (blank, opt))
            got, rc, dt = run_exe(exe, N_TIME)
            print("  题%d 选项%s @ n=%d: %.2fs exit=%d" % (QNO[blank], opt, N_TIME, dt, rc))
