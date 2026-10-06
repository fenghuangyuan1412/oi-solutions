/*
 * P11232 [CSP-S 2024] 超速检测 —— 骗分 / 暴力版
 * ---------------------------------------------------------------------------
 * 与正解【同一套整数判定】，只是不做排序、不二分：
 *   每辆车枚举全部 m 个测速仪，逐个判断"位置在 [d_i, L] 且 g(p) > V^2"。
 *   超速车区间取枚举到的最小/最大下标（合法测速仪在排序后是连续一段）。
 * 复杂度 O(n*m)：n,m<=10（样例2）、n,m<=3000（部分档）能过，
 *   n=m=1e5 时 1e10 次运算必然超时。
 *
 * 编译：g++ -static -O2 -std=c++14 solution_partial.cpp -o partial.exe
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n, m;
        ll L, V;
        cin >> n >> m >> L >> V;
        vector<ll> d(n), v(n), a(n);
        for (int i = 0; i < n; ++i) cin >> d[i] >> v[i] >> a[i];
        vector<ll> p(m);
        for (int j = 0; j < m; ++j) cin >> p[j];
        sort(p.begin(), p.end());                 // 仍需排序，区间才连续

        ll V2 = V * V;
        vector<pair<int,int>> segs;
        int speedCnt = 0;

        for (int i = 0; i < n; ++i) {
            int A = m, B = -1;                    // 合法测速仪下标区间
            for (int j = 0; j < m; ++j) {
                if (p[j] < d[i] || p[j] > L) continue;
                ll gv = v[i] * v[i] + 2LL * a[i] * (p[j] - d[i]);
                if (gv > V2) { A = min(A, j); B = max(B, j); }
            }
            if (A <= B) { ++speedCnt; segs.push_back({A, B}); }
        }

        sort(segs.begin(), segs.end(),
             [](const pair<int,int>& x, const pair<int,int>& y) {
                 return x.second < y.second;
             });
        int keep = 0, lastPt = -1;
        for (auto& s : segs)
            if (s.first > lastPt) { ++keep; lastPt = s.second; }

        cout << speedCnt << " " << (m - keep) << "\n";
    }
    return 0;
}
