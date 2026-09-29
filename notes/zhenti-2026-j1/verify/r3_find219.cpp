// r3_find219.cpp —— 反查：哪些 01 串会让原文版 dfs 恰好被调用 219 次？
// （用来判断官方第 31 题的 "219 次" 到底对应哪个输入）
// 编译： g++ -static -O2 -std=c++14 r3_find219.cpp -o r3_find219.exe
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>
using namespace std;
int n;
string s;
long long calls;
const long long CAP = 219;       // 只关心"恰好 219 次"，一旦超过立刻放弃该串
const int INF = 1000000000;

int dfs(string t, int lst, int deep) {
    if (++calls > CAP) return INF;
    int ret = 0;
    for (int i = 0; i <= n; i++) if (t[i] != '1') ret = INF;
    if (ret == 0) return 0;
    for (int i = 0; i <= n; i++) {
        if (calls > CAP) return INF;
        if (i != lst && s.substr(n - i, i) == t.substr(0, i)) {
            string cur = t;
            cur[i] ^= 1;
            ret = min(ret, dfs(cur, i, deep + 1) + 1);
        }
    }
    return ret;
}

int main() {
    for (int len = 1; len <= 18; len++) {
        int hit = 0;
        for (int mask = 0; mask < (1 << len); mask++) {
            s.clear();
            for (int i = 0; i < len; i++) s += char('0' + ((mask >> (len - 1 - i)) & 1));
            n = len;
            calls = 0;
            string t0(n + 1, '0');
            int a = dfs(t0, -1, 0);
            if (calls == 219) {
                if (hit < 40) printf("len=%2d  s=%s  ans=%d  calls=%lld\n", len, s.c_str(), a, calls);
                hit++;
            }
        }
        printf("--- len=%d 共 %d 个串调用次数恰好为 219\n", len, hit);
        fflush(stdout);
    }
    return 0;
}
