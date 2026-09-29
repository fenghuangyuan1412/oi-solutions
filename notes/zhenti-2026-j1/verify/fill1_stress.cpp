// fill1_stress.cpp —— 完善程序(1) 序列第 k 小的验证
//   1) 随机 5000 组，与 sort 后取第 k 小对拍（④ 填 D 的正确版）
//   2) ③ 处填 2e9 到底会不会 int 溢出（实测 double->int 转换、1<<32 的值）
//   3) ④ 处若填 A (l+r)/2：在 a[i] 接近 2e9 时 l+r 溢出，实测会不会算错 / 死循环
//   4) ② 处填反方向 (c<=k) 会怎样
// 编译： g++ -static -O2 -std=c++14 fill1_stress.cpp -o fill1_stress.exe
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <cstdio>
#include <climits>
using namespace std;

int n, k;
vector<int> a;
long long steps;          // 二分迭代次数，用来抓死循环

bool check(int x) { int c = 0; for (int i = 0; i < n; i++) if (a[i] <= x) c++; return k <= c; }

int solveD() {                                   // ④ 填 D: l + (r - l) / 2
    int l = 1, r = 2e9; steps = 0;
    while (l < r) {
        if (++steps > 200) return -700000000;    // 死循环保护
        int x = l + (r - l) / 2;
        if (check(x)) r = x; else l = x + 1;
    }
    return l;
}
int solveA() {                                   // ④ 填 A: (l + r) / 2   —— 有溢出风险
    int l = 1, r = 2e9; steps = 0;
    while (l < r) {
        if (++steps > 200) return -800000000;    // 死循环保护
        int x = (l + r) / 2;
        if (check(x)) r = x; else l = x + 1;
    }
    return l;
}
int solveC3() {                                  // ③ 填 C: 1 << 32
    volatile int sh = 32;
    int l = 1, r = 1 << sh;
    printf("   [③填C] r = 1<<32 的实际值 = %d\n", r);
    steps = 0;
    while (l < r) {
        if (++steps > 200) return -900000000;
        int x = l + (r - l) / 2;
        if (check(x)) r = x; else l = x + 1;
    }
    return l;
}
int solveB3() {                                  // ③ 填 B: a[n]（没排序，可能偏小）
    int l = 1, r = a[n - 1];
    steps = 0;
    while (l < r) {
        if (++steps > 200) return -900000000;
        int x = l + (r - l) / 2;
        if (check(x)) r = x; else l = x + 1;
    }
    return l;
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("--- 0) 字面量本身 ---\n");
    printf("  INT_MAX = %d ; (double)2e9 = %.0f ; int r = 2e9 -> r = %d\n", INT_MAX, 2e9, (int)(double)2e9);
    printf("  2e9 = 2000000000 < 2147483647 => ③ 填 2e9 本身不溢出\n");
    volatile int sh = 32, sh31 = 31;
    printf("  1 << 32 = %d (移位>=32 是 UB) ; 1 << 31 = %d (有符号溢出 UB)\n", 1 << sh, 1 << sh31);
    printf("  二分中 l 和 r 都可能取到 2e9 => (l + r) 最大约 %lld，远超 INT_MAX\n", 2000000000LL * 2);

    printf("\n--- 1) 随机 5000 组对拍 (n<=50, 1<=a[i]<=2e9) ---\n");
    mt19937 rng(12345);
    int badD = 0, badA = 0, hangA = 0, diffDA = 0;
    for (int t = 0; t < 5000; t++) {
        n = 1 + (int)(rng() % 50);
        k = 1 + (int)(rng() % n);
        a.assign(n, 0);
        for (int i = 0; i < n; i++) a[i] = 1 + (int)(rng() % 2000000000);
        vector<int> s = a;
        sort(s.begin(), s.end());
        int ref = s[k - 1];
        int d = solveD(), ab = solveA();
        if (d != ref) { if (badD < 5) printf("  D版错: ref=%d got=%d\n", ref, d); badD++; }
        if (ab != ref) { if (badA < 5) printf("  A版错: ref=%d got=%d\n", ref, ab); badA++; }
        if (ab == -800000000) hangA++;
        if (d != ab) diffDA++;
    }
    printf("  ④填D：与 sort 结果不一致 = %d / 5000\n", badD);
    printf("  ④填A：与 sort 结果不一致 = %d / 5000 (其中死循环被强制截断 = %d)\n", badA, hangA);
    printf("  两版结果不同的组数 = %d\n", diffDA);

    printf("\n--- 2) 极端数据：a[i] 全部接近 2e9 ---\n");
    n = 5; k = 1; a.assign(n, 2000000000);
    printf("  n=5 k=1 a={2e9,2e9,2e9,2e9,2e9} 正确=2000000000 : D->%d  A->%d (A 迭代 %lld 次)\n",
           solveD(), solveA(), steps);
    n = 5; k = 5; a = {1, 2, 3, 2000000000, 1999999999};
    printf("  n=5 k=5 a={1,2,3,2e9,1999999999} 正确=2000000000 : D->%d  A->%d\n", solveD(), solveA());
    n = 4; k = 4; a = {1500000000, 1600000000, 1700000000, 2000000000};
    printf("  n=4 k=4 a={1.5e9,1.6e9,1.7e9,2e9} 正确=2000000000 : D->%d  A->%d\n", solveD(), solveA());

    printf("\n--- 3) ③ 处填 C (1<<32) 或 B (a[n]) ---\n");
    n = 5; k = 3; a = {7, 2, 9, 1, 5};
    printf("  a={7,2,9,1,5} k=3 正确=5 : D->%d  C->%d  B(a[n]=5)->%d\n", solveD(), solveC3(), solveB3());
    n = 5; k = 3; a = {5, 1, 9, 2, 3};        // a[n]=3 比真实第3小=5 小
    printf("  a={5,1,9,2,3} k=3 正确=5 : D->%d  B(a[n]=3)->%d\n", solveD(), solveB3());
    n = 3; k = 1; a = {1500000000, 2000000000, 1999999999};
    printf("  a={1.5e9,2e9,1999999999} k=1 正确=1500000000 : D->%d\n", solveD());

    printf("\n--- 4) ② 处填反方向 ---\n");
    n = 5; k = 3; a = {7, 2, 9, 1, 5};
    {
        int l = 1, r = 2e9, st = 0;
        while (l < r && ++st <= 200) {
            int x = l + (r - l) / 2, c = 0;
            for (int i = 0; i < n; i++) if (a[i] <= x) c++;
            if (c <= k) r = x; else l = x + 1;
        }
        printf("  ②填A(c<=k) -> %d  (正确 5) ; 迭代 %d 次\n", l, st);
    }
    {
        int l = 1, r = 2e9, st = 0;
        while (l < r && ++st <= 200) {
            int x = l + (r - l) / 2, c = 0;
            for (int i = 0; i < n; i++) if (a[i] < x) c++;     // ① 填 B
            if (k <= c) r = x; else l = x + 1;
        }
        printf("  ①填B(a[i]<x) -> %d  (正确 5) ; 迭代 %d 次\n", l, st);
    }
    return 0;
}
