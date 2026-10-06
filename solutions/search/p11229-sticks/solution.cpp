// P11229 [CSP-J 2024] 小木棍 —— 正解：最少位数 + 逐位贪心（可行性判定）
// 编译：g++ -static -O2 -std=c++14 solution.cpp -o <输出>
#include <bits/stdc++.h>
using namespace std;

// 七段数码管摆每个数字需要的木棍数（题面图）
const int C[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int n;
        scanf("%d", &n);
        if (n == 1) { puts("-1"); continue; }   // 一根木棍什么都摆不出
        int L = (n + 6) / 7;                    // 位数下界 ceil(n/7)
        if (2 * L > n) { puts("-1"); continue; } // 防御：位数太少了装不下（实际只有 n=1 会走到）
        string s;
        int rem = n;                            // 剩余木棍
        for (int pos = 0; pos < L; ++pos) {
            int slots = L - pos - 1;            // 这一位选定后还剩几个空位
            for (int d = (pos == 0 ? 1 : 0); d <= 9; ++d) {
                int r = rem - C[d];
                if (r < 2 * slots || r > 7 * slots) continue; // 剩余木棍填不满/装不下
                s.push_back(char('0' + d));
                rem = r;
                break;
            }
        }
        puts(s.c_str());
    }
    return 0;
}
