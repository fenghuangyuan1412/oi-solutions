/*
 * P11232 [CSP-S 2024] 超速检测 —— 正解
 * ---------------------------------------------------------------------------
 * 一辆车在某位置 x 的瞬时速度平方（整数，无浮点）：
 *     g(x) = v^2 + 2*a*(x - d)
 *   判定"超速"就是 g(x) > V^2 —— 两边都是整数，直接比，避开精度坑。
 *
 * 第一问：所有测速仪都开，有几辆车被判定超速。
 * 第二问：每辆超速车的"能抓到它的测速仪"恰好构成一段【连续区间】
 *         （因为 g 对 x 单调），留下最少的测速仪把每段都点到 = 经典
 *         【区间打点贪心】：按右端点排序，遇到没覆盖的就钉在右端点。
 *
 * 编译：g++ -static -O2 -std=c++14 solution.cpp -o out.exe
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
        sort(p.begin(), p.end());                 // 测速仪按位置排序

        ll V2 = V * V;
        // g(i, x) = v^2 + 2a(x-d)：车 i 在位置 x 的速度平方（整数）
        auto g = [&](int i, ll x) -> ll {
            return v[i] * v[i] + 2LL * a[i] * (x - d[i]);
        };

        vector<pair<int, int>> segs;              // 每辆超速车对应的测速仪下标区间
        int speedCnt = 0;

        for (int i = 0; i < n; ++i) {
            // 车在路上的位置范围 [d_i, L]（驶入/驶出端点也测）
            int lo = (int)(lower_bound(p.begin(), p.end(), d[i]) - p.begin());
            int hi = (int)(upper_bound(p.begin(), p.end(), L) - p.begin()) - 1;
            if (lo > hi) continue;                // 路上没有测速仪

            int A = lo, B = hi;                   // 超速区间（下标）
            if (a[i] > 0) {                       // g 递增：后缀，找第一个 g>V2
                if (g(i, p[hi]) <= V2) continue;
                int l = lo, r = hi, ans = hi;
                while (l <= r) {
                    int mid = (l + r) >> 1;
                    if (g(i, p[mid]) > V2) { ans = mid; r = mid - 1; }
                    else l = mid + 1;
                }
                A = ans; B = hi;
            } else if (a[i] < 0) {                // g 递减：前缀，找最后一个 g>V2
                if (g(i, p[lo]) <= V2) continue;
                int l = lo, r = hi, ans = lo;
                while (l <= r) {
                    int mid = (l + r) >> 1;
                    if (g(i, p[mid]) > V2) { ans = mid; l = mid + 1; }
                    else r = mid - 1;
                }
                A = lo; B = ans;
            } else {                              // a==0：全程恒速
                if (v[i] * v[i] <= V2) continue;
                A = lo; B = hi;
            }
            if (A > B) continue;
            ++speedCnt;
            segs.push_back({A, B});
        }

        // 第二问：区间打点贪心，求最少留几个测速仪
        sort(segs.begin(), segs.end(),
             [](const pair<int,int>& x, const pair<int,int>& y) {
                 return x.second < y.second;
             });
        int keep = 0, lastPt = -1;
        for (auto& s : segs) {
            if (s.first > lastPt) {               // 当前区间还没被点到
                ++keep;
                lastPt = s.second;                // 钉在右端点
            }
        }
        cout << speedCnt << " " << (m - keep) << "\n";
    }
    return 0;
}
