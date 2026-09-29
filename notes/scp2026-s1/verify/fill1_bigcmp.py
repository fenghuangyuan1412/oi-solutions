# fill1_bigcmp.py -- 在 n=1e7（满规模）下逐字节比对：官方答案版 / 各"输出也正确"的选项 / 朴素 qpow
# 两个程序的输出格式都是 printf("%d ", x)，所以直接对 stdout 原始字节求哈希即可判断是否完全一致
import subprocess, os, hashlib, time

D = os.path.dirname(os.path.abspath(__file__))
N = int(os.environ.get("CMPN", 10000000))


def digest(exe, n=N):
    t0 = time.time()
    r = subprocess.run([exe], input=("%d\n" % n).encode(), stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    h = hashlib.sha1(r.stdout).hexdigest()
    return h, len(r.stdout), r.returncode, time.time() - t0, r.stdout[:24].decode("ascii", "replace")


def brute(n):
    exe = os.path.join(D, "fill1_brute.exe")
    return digest(exe, n)


if __name__ == "__main__":
    hb, lb, rc, dt, head = brute(N)
    print("朴素 fill1_brute @ n=%d: sha1=%s len=%d exit=%d %.1fs 开头=%s" % (N, hb[:16], lb, rc, dt, head))
    print()
    list_ = [("官方答案 34A35D36B37B38C", "fill1.exe"),
             ("34 A(=官方)", "fill1_v_B1A.exe"),
             ("35 D(=官方)", "fill1_v_B2D.exe"),
             ("36 A", "fill1_v_B3A.exe"),
             ("36 B(=官方)", "fill1_v_B3B.exe"),
             ("37 B(=官方)", "fill1_v_B4B.exe"),
             ("38 A", "fill1_v_B5A.exe"),
             ("38 B", "fill1_v_B5B.exe"),
             ("38 C(=官方)", "fill1_v_B5C.exe"),
             ("38 D", "fill1_v_B5D.exe")]
    for name, fn in list_:
        p = os.path.join(D, fn)
        if not os.path.exists(p):
            print("%-24s 程序不存在 %s" % (name, fn))
            continue
        h, l, rc, dt, head = digest(p)
        print("%-24s sha1=%s len=%d exit=%d %.2fs  %s  开头=%s" % (
            name, h[:16], l, rc, dt, "与朴素完全一致" if h == hb else "!!! 与朴素不一致", head))
    print()
    # 38 各选项的内层循环/qpow 调用次数（计数版）
    for tag in ["B3B", "B5A", "B5B", "B5C", "B5D"]:
        p = os.path.join(D, "fill1_cnt_%s.exe" % tag)
        if not os.path.exists(p):
            print("  计数版 %s 不存在" % tag)
            continue
        r = subprocess.run([p], input=("%d\n" % N).encode(), stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
        print("  计数版 %-4s @ n=%d: %s" % (tag, N, r.stderr.decode("utf-8", "replace").strip()))
