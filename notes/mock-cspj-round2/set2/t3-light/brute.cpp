#include <bits/stdc++.h>
using namespace std;

int cost[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};
string g[100005];    // g[s] = 恰好花 s 颗 LED 的"数字串"（允许开头是 0），先比长度再比字典序
bool ok[100005];

bool better(const string& a, const string& b, bool bvalid) {
    if (!bvalid) return true;
    if (a.size() != b.size()) return a.size() < b.size();
    return a < b;
}

// 暴力：纯 DP 枚举每一位放什么数字，和贪心的思路完全不同
int main() {
    int n;
    scanf("%d", &n);
    ok[0] = true;
    g[0] = "";
    for (int s = 1; s <= n; s++) {
        for (int d = 0; d <= 9; d++) {
            int c = s - cost[d];
            if (c < 0 || !ok[c]) continue;
            string cand = char('0' + d) + g[c];
            if (better(cand, g[s], ok[s])) {
                g[s] = cand;
                ok[s] = true;
            }
        }
    }
    string ans = "";
    bool found = false;
    for (int d = 1; d <= 9; d++) {  // 首位不能是 0
        int c = n - cost[d];
        if (c < 0 || !ok[c]) continue;
        string cand = char('0' + d) + g[c];
        if (better(cand, ans, found)) {
            ans = cand;
            found = true;
        }
    }
    puts(found ? ans.c_str() : "-1");
    return 0;
}
