/*
 * P9753 消消乐 —— 暴力档（n<=10）+ 中间档（区间 DP）阶梯版
 * 详见同目录 README.md「一、骗分：这一档怎么拿」
 * 编译：g++ -static -O2 -std=c++14 solution_partial.cpp -o out.exe
 */
#include <bits/stdc++.h>
using namespace std;

static int n;
static string s;

/* ---------- 判一个串能不能消空：纯 DFS，无记忆化 ---------- */
static bool canElim(const string& t) {
    if (t.empty()) return true;
    if (t.size() & 1) return false;                 // 每次删 2 个 => 奇数长度永远删不空
    for (int i = 1; i < (int)t.size(); ++i) {
        if (t[i] == t[i - 1]) {                     // 找一对相邻相同字符
            string u = t.substr(0, i - 1) + t.substr(i + 1);
            if (canElim(u)) return true;            // 删掉它，递归看剩下的能不能消空
        }
    }
    return false;                                   // 一对都删不掉（或每种删法都失败）
}

/* ---------- 档 1：枚举所有非空连续子串，逐个 DFS ---------- */
static long long bruteAll() {
    long long ans = 0;
    for (int l = 0; l < n; ++l)
        for (int r = l; r < n; ++r)
            if (canElim(s.substr(l, r - l + 1))) ++ans;
    return ans;
}

/* ---------- 档 2：区间 DP ----------
 * row[l][r] = 子串 s[l..r]（下标从 1 起，闭区间）是否可消除。
 * 观察：若 [l..r] 可消除，看 l 最后一次被删时和谁配对，设配到 k（l<k<=r）：
 *       则 s[k]==s[l]、[l+1..k-1] 可消空、且 [k+1..r] 可消空（空区间算可消空）。
 * 于是：先由 "s[k]==s[l] && [l+1..k-1] 可消空" 得到 [l..k] 可消空，
 *       再把 [l..k] 后面接上任意一个"[k+1..r] 可消空"的 r。
 * 行从下往上推（l 从 n 到 1），被引用的行都比当前行短，一定已经算好。
 */
static long long intervalDP() {
    vector<vector<char>> row(n + 3, vector<char>(n + 3, 0));   // row[l][r]
    for (int l = n; l >= 1; --l) {
        for (int k = l + 1; k <= n; ++k) {
            if (s[k - 1] != s[l - 1]) continue;                 // 字母不同配不了对
            if (k > l + 1 && !row[l + 1][k - 1]) continue;      // 中间消不空
            row[l][k] = 1;                                      // [l..k] 自己可消空
            for (int r = k + 2; r <= n; r += 2)                 // 后面接一段可消空的
                if (row[k + 1][r]) row[l][r] = 1;
        }
    }
    long long ans = 0;
    for (int l = 1; l <= n; ++l)
        for (int r = l; r <= n; ++r) ans += row[l][r];
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> s;
    if (n <= 10) { cout << bruteAll() << "\n"; return 0; }
    cout << intervalDP() << "\n";
    return 0;
}
