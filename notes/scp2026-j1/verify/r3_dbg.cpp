// r3_dbg.cpp —— 排查第 3 篇程序为什么会崩：
//   1) 记录"当前递归栈"上的状态，真正的环 = 同一个 (t,lst) 在栈上出现两次
//   2) 深度超过 LIMIT 就直接把栈打出来并退出，防止把栈撑爆
// 用法: echo 110111011110111110 | ./r3_dbg.exe
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <cstdio>
using namespace std;
int n;
string s;
long long calls = 0;
const int LIMIT = 260;
vector<string> stk;                 // 当前递归栈上的 (t,lst)
set<string> onstk;
FILE* logfp = 0;

int dfs(string t, int lst, int deep = 0) {
    calls++;
    string key = t + "|" + to_string(lst);
    if (onstk.count(key)) {
        fprintf(stdout, "!!! 真正的环: 状态 (%s) 已经在当前递归栈上 —— 递归永远不会返回\n", key.c_str());
        fprintf(stdout, "总调用次数到崩溃前 = %lld\n", calls);
        fprintf(stdout, "栈 (从入口到当前位置):\n");
        for (size_t i = 0; i < stk.size(); ++i) fprintf(stdout, "%3zu  %s\n", i, stk[i].c_str());
        fflush(stdout);
        exit(3);
    }
    if (deep > LIMIT) {
        fprintf(stdout, "!!! 深度超过 %d 还没返回，强行截断。总调用 = %lld\n栈:\n", LIMIT, calls);
        for (size_t i = 0; i < stk.size(); ++i) fprintf(stdout, "%3zu  %s\n", i, stk[i].c_str());
        fflush(stdout);
        exit(4);
    }
    stk.push_back(key); onstk.insert(key);
    int ret = 0;
    for (int i = 0; i <= n; i++)
        if (t[i] != '1')
            ret = 1e9;
    if (ret == 0) { stk.pop_back(); onstk.erase(key); return 0; }
    for (int i = 0; i <= n; i++)
        if (i != lst && s.substr(n - i, i) == t.substr(0, i)) {
            string cur = t;
            cur[i] ^= 1;
            ret = min(ret, dfs(cur, i, deep + 1) + 1);
        }
    stk.pop_back(); onstk.erase(key);
    return ret;
}
int main() {
    cin >> s;
    n = s.size();
    string t0(n + 1, '0');
    printf("s = %s   n = %d\n", s.c_str(), n);
    for (int i = 0; i <= n; i++)
        printf("  翻第 %2d 位需要: t[0..%d] == \"%s\"\n", i, i - 1, s.substr(n - i, i).c_str());
    fflush(stdout);
    int ans = dfs(t0, -1);
    printf("answer=%d calls=%lld\n", ans, calls);
    fflush(stdout);
    return 0;
}
