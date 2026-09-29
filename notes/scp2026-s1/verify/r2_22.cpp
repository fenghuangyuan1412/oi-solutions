// r2_22.cpp -- 题 22：solve1 朴素字符串拼接的真实代价
// 统计：拼接次数、每次新串长度、总拷贝字符数、总耗时；n = 1e4 / 1e5 / 1e6 / 4e6
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll tot_chars = 0; int cnt_concat = 0; ll max_len = 0;
vector<ll> lens;
int solve1(int n, int p) {
    string s = "0", nxt = "01", tmp;
    while (nxt.length() < n) {
        tmp = nxt + s;                 // 第 20 行：一次拼接
        lens.push_back((ll)tmp.length());
        tot_chars += (ll)tmp.length(); // 新串长度 = 本次拷贝的字符数（operator+ 至少这么多）
        cnt_concat++;
        max_len = max(max_len, (ll)tmp.length());
        s = nxt;                       // 又一次整串拷贝
        tot_chars += (ll)nxt.length();
        nxt = tmp;                     // 又一次整串拷贝（移动语义下可忽略，这里两种都统计）
    }
    int ans = 0;
    for (int i = p; i < n; i += 2) ans += nxt[i] - '0';
    return ans;
}
int main() {
    printf("%10s %8s %10s %20s %8s %12s\n",
           "n", "concat", "maxlen", "total_chars", "n(logn)", "ms");
    for (int n : {10000, 100000, 1000000, 4000000}) {
        tot_chars = 0; cnt_concat = 0; max_len = 0; lens.clear();
        auto t0 = chrono::steady_clock::now();
        int r = solve1(n, 0);
        auto t1 = chrono::steady_clock::now();
        double ms = chrono::duration<double, milli>(t1 - t0).count();
        printf("%10d %8d %10lld %20lld %12.1f %12.2f  ans=%d\n",
               n, cnt_concat, max_len, tot_chars, n * log2((double)n), ms, r);
    }
    // 逐次拼接长度序列（n=1e6）
    tot_chars = 0; cnt_concat = 0; lens.clear();
    solve1(1000000, 0);
    printf("concat lengths (n=1e6): ");
    for (size_t i = 0; i < lens.size(); i++) printf("%lld ", lens[i]);
    printf("\nsum(lens)=%lld  ratio sum/n=%.4f  2.618*n=%.1f\n",
           tot_chars, (double)tot_chars / 1000000.0, 2.618 * 1000000);
    return 0;
}
