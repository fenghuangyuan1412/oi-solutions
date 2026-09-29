// r3_cnt.cpp —— 与 r3.cpp 逐字相同，只是加了两个全局计数器：
//   calls   : dfs 被调用的总次数
//   states  : 出现过的不同 (t, lst) 状态数
//   maxdeep : 最深递归深度
// 用法: echo 110111011110111110 | ./r3_cnt.exe
#include <iostream>
#include <string>
#include <algorithm>
#include <set>
using namespace std;
int n;
string s;
long long calls = 0, maxdeep = 0, steps = 0;
set<string> seen;

int dfs(string t, int lst, int deep = 0) {
    calls++;                                   // <== 新增
    seen.insert(t + "|" + to_string(lst));     // <== 新增
    if (deep > maxdeep) maxdeep = deep;        // <== 新增
    int ret = 0;
    for (int i = 0; i <= n; i++)
        if (t[i] != '1')
            ret = 1e9;
    if (ret == 0) return 0;
    for (int i = 0; i <= n; i++)
        if (i != lst && s.substr(n - i, i) == t.substr(0, i)) {
            string cur = t;
            cur[i] ^= 1;
            ret = min(ret, dfs(cur, i, deep + 1) + 1);
        }
    return ret;
}
int main() {
    cin >> s;
    n = s.size();
    string t0(n + 1, '0');
    int ans = dfs(t0, -1);
    cout << "\n[answer]=" << ans << "\n[calls]=" << calls << "\n[distinct states]=" << seen.size()
         << "\n[max recursion depth]=" << maxdeep << "\n";
    return 0;
}
