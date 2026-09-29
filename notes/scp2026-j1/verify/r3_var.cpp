// r3_var.cpp —— 第 3 篇程序的所有变体，用命令行参数选择模式位
//   MODE 1 = 原题（与 r3.cpp 完全一致）
//   MODE 2 = 把第 11 行 1e9 换成 1234567            (第 27 题)
//   MODE 4 = 删掉第 14 行的 i != lst &&              (第 28 题)
//   MODE 8 = 第 16 行 cur[i] ^= 1 改为 cur[i] = 'a' - cur[i];  (第 29 题 C)
//   可叠加，例如 MODE=3 表示同时改 1e9 和去掉 guard
// 用法: echo 00000001 | ./r3_var.exe 1
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
using namespace std;
int n, MODE;
string s;
long long calls = 0;

int dfs(string t, int lst) {
    calls++;
    int ret = 0;
    const int INF = (MODE & 2) ? 1234567 : 1000000000;   // 1e9
    for (int i = 0; i <= n; i++)
        if (t[i] != '1')
            ret = INF;
    if (ret == 0) return 0;
    for (int i = 0; i <= n; i++) {
        bool guard_ok = (MODE & 4) ? true : (i != lst);
        if (guard_ok && s.substr(n - i, i) == t.substr(0, i)) {
            string cur = t;
            if (MODE & 8) cur[i] = 'a' - cur[i];
            else cur[i] ^= 1;
            ret = min(ret, dfs(cur, i) + 1);
        }
    }
    return ret;
}

int main(int argc, char** argv) {
    MODE = (argc > 1) ? atoi(argv[1]) : 1;
    cin >> s;
    n = s.size();
    string t0(n + 1, '0');
    int ans = dfs(t0, -1);
    cout << "\n[MODE=" << MODE << "][answer]=" << ans << " [calls]=" << calls << "\n";
    return 0;
}
