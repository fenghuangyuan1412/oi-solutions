// verify/brute.cpp —— 子集枚举暴力（仅用于验证，非讲解代码）
// n ≤ 16 时枚举全部 2^n 个"移走集合"，逐个小朋友按题面条件直接判定。
// 与正解的状压 DP 完全独立，连窗口相对位都不用。
// 编译：g++ -static -O2 -std=c++14 brute.cpp -o brute.exe
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, c;
    if (scanf("%d%d", &n, &c) != 2) return 0;
    vector<int> fear(c, 0), like(c, 0);
    for (int i = 0; i < c; i++) {
        int E, F, L;
        scanf("%d%d%d", &E, &F, &L);
        for (int j = 0; j < F; j++) { int x; scanf("%d", &x); fear[i] |= 1 << (x - 1); }
        for (int j = 0; j < L; j++) { int y; scanf("%d", &y); like[i] |= 1 << (y - 1); }
    }
    if (n > 16) { printf("-1\n"); return 0; }
    int best = 0;
    for (int S = 0; S < (1 << n); S++) {
        int h = 0;
        for (int i = 0; i < c; i++)
            if ((fear[i] & S) || (like[i] & ~S)) h++;
        if (h > best) best = h;
    }
    printf("%d\n", best);
    return 0;
}
