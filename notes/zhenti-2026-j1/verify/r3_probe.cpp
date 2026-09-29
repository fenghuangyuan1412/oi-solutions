// r3_probe.cpp —— 取证：原文提取出的第 3 篇程序在 n=18 时会把系统栈撑爆（递归深度 289745），
// 而官方答案说 dfs 只被调用 219 次。这里枚举若干"最可能在 PDF 里被提取错"的写法变体，
// 看哪一种能同时满足官方给的三个数字（341 / 219次 / 既不是21也不是289746也不是525739）。
// 编译需要大栈： g++ -static -O2 -std=c++14 -Wl,--stack,1073741824 r3_probe.cpp -o r3_probe.exe
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>
using namespace std;

int n;
string s;
long long calls, maxdeep;
int V;          // 变体编号
const int INF = 1000000000;

int hi1() { return (V == 1 || V == 3 || V == 6) ? n - 1 : n; }   // 第 9 行循环上界
int hi2() { return (V == 2 || V == 3 || V == 6) ? n - 1 : n; }   // 第 13 行循环上界
char goalChar() { return '1'; }

int dfs(string t, int lst, int deep) {
    calls++;
    if (deep > maxdeep) maxdeep = deep;
    if (calls > 40000000LL) { fprintf(stderr, "calls>4e7 abort\n"); exit(9); }
    int ret = 0;
    for (int i = 0; i <= hi1(); i++)
        if (t[i] != goalChar())
            ret = INF;
    if (ret == 0) return 0;
    for (int i = 0; i <= hi2(); i++) {
        string need = (V == 5) ? s.substr(0, i) : s.substr(n - i, i);
        if (i != lst && need == t.substr(0, i)) {
            string cur = t;
            cur[i] ^= 1;
            ret = min(ret, dfs(cur, i, deep + 1) + 1);
        }
    }
    return ret;
}

void run(int v, const char* tag) {
    V = v;
    struct Case { const char* in; };
    const char* ins[] = {"00000001", "110111011110111110", "001000100001000001",
                         "10011101010", "01100010101"};
    printf("== 变体 %d (%s) ==\n", v, tag);
    for (const char* in : ins) {
        s = in; n = s.size();
        string t0((V == 4) ? n : (n + 1), '0');
        calls = 0; maxdeep = 0;
        int a = dfs(t0, -1, 0);
        printf("  s=%-20s n=%2d ans=%-10d calls=%-9lld maxdeep=%lld\n", in, n, a, calls, maxdeep);
        fflush(stdout);
    }
}

int main() {
    run(0, "原文逐字版");
    run(1, "第9行 i<n");
    run(2, "第13行 i<n");
    run(3, "两处都 i<n");
    run(5, "s.substr(0,i)");
    return 0;
}
