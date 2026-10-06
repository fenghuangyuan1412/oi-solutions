/*
 * P11230 [CSP-J 2024] 接龙 —— 骗分 / 小数据精确版
 * ---------------------------------------------------------------------------
 * 把每一轮看成"某个词库里一段长度 ∈[2,k] 的连续子串"，子串首尾相接、且相邻
 * 两轮不能是同一个人。给定 (r, c) 问是否存在这样的 r 轮过程。
 *
 * 本文件是【精确 DFS + 记忆化】：枚举每一轮选谁、接成什么结尾。
 *   · r = 1 的档（测试点 1）：直接一轮判定，n 可到 1e5 也没问题；
 *   · n<=10, r<=5（测试点 2,3）与 n<=1000, r<=10（性质 A/B/C 的小档）：
 *     DFS 能算出正确答案；
 *   · 但状态里带"上一个人是谁"，分支随 n、r 指数增长，
 *     n=1e5, r=1e2 的大档【必然超时/爆栈】，那正是正解要用图论 BFS 解决的部分。
 * 官方样例在本文件下输出与题面逐字一致（见 README 实测）。
 *
 * 编译：g++ -static -O2 -std=c++14 solution_partial.cpp -o partial.exe
 */
#include <bits/stdc++.h>
using namespace std;

int n;
long long k;
vector<unordered_map<int, unordered_set<int>>> reach; // reach[i][s] = 以 s 开头能接到的结尾集合
unordered_set<int> canOne;  // 一轮之内（从值 1 出发）能接到的所有结尾值

// 记忆化：(depth, curVal, lastPerson) 是否已在某条成功路径上访问过
unordered_map<long long, char> memo;

bool dfs(int depth, int startVal, int lastPerson, int R, int c) {
    long long key = ((long long)depth << 42) ^ ((long long)(startVal + 1) << 21) ^ (lastPerson + 1);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    bool ok = false;
    // 第 depth 轮（0 基）：选一个不是 lastPerson 的人，从 startVal 接出一段
    for (int j = 0; j < n && !ok; ++j) {
        if (j == lastPerson) continue;
        auto& mp = reach[j];
        auto f = mp.find(startVal);
        if (f == mp.end()) continue;
        for (int e : f->second) {
            if (depth == R - 1) {            // 这是最后一轮，结尾必须等于 c
                if (e == c) { ok = true; break; }
            } else if (dfs(depth + 1, e, j, R, c)) {
                ok = true; break;
            }
        }
    }
    memo[key] = ok;
    return ok;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        long long q;
        cin >> n >> k >> q;
        reach.assign(n, {});
        canOne.clear();
        for (int i = 0; i < n; ++i) {
            int l;
            cin >> l;
            vector<int> s(l);
            for (int t = 0; t < l; ++t) cin >> s[t];
            // 枚举所有长度 ∈[2,k] 的连续子串
            for (int st = 0; st < l; ++st) {
                for (int len = 2; len <= k && st + len <= l; ++len) {
                    int a = s[st], b = s[st + len - 1];
                    reach[i][a].insert(b);
                    if (a == 1) canOne.insert(b); // 第一轮从 1 出发，一轮可达的结尾
                }
            }
        }
        for (int qq = 0; qq < q; ++qq) {
            int r, c;
            cin >> r >> c;
            if (r == 1) {                       // 测试点 1：单轮，O(1) 命中
                cout << (canOne.count(c) ? 1 : 0) << "\n";
                continue;
            }
            memo.clear();
            // 第一轮：起始值必须是 1，没有"上一个人"限制（lastPerson = -1）
            cout << (dfs(0, 1, -1, r, c) ? 1 : 0) << "\n";
        }
    }
    return 0;
}
