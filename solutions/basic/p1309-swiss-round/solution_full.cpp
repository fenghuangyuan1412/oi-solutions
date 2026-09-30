#include <bits/stdc++.h>
using namespace std;

const int N = 200005;

struct Player { int id, s, w; };

Player a[N], b[N], win[N], lose[N];   // a=当前排名序，win/lose=本轮胜者/负者两条队列
int n, r, q;

bool cmp(const Player& x, const Player& y) {
    if (x.s != y.s) return x.s > y.s;
    return x.id < y.id;
}

// 满分做法：归并代替排序，O(R·N)
// 依据：本轮开始前 a[] 已按"分数降序、同分编号升序"排好。
//   胜者集合 = 每对里赢的那个，他们的分数各自 +1。
//   把胜者按原顺序排出来，两两比较：设第 i 对胜者来自位置 2i-1 或 2i，
//   因为原序是有序的，胜者队列天然有序 —— 分数上：先出现的胜者分数 ≥ 后出现的胜者分数
//   （原序分数不增，且都 +1）；同分时编号也不增（原序同分按编号升序，取的仍是靠前者）。
//   负者队列同理。于是两条队列都已有序，一次归并 = 新的全局序。
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> r >> q;
    for (int i = 1; i <= 2 * n; i++) { cin >> a[i].s; a[i].id = i; }
    for (int i = 1; i <= 2 * n; i++) cin >> a[i].w;

    sort(a + 1, a + 2 * n + 1, cmp);

    for (int round = 1; round <= r; round++) {
        int nw = 0, nl = 0;
        for (int i = 1; i <= 2 * n; i += 2) {
            if (a[i].w > a[i + 1].w) { a[i].s += 1;   win[++nw] = a[i];   lose[++nl] = a[i + 1]; }
            else                     { a[i + 1].s += 1; win[++nw] = a[i + 1]; lose[++nl] = a[i]; }
        }
        // 归并两条有序队列
        int i = 1, j = 1, k = 0;
        while (i <= nw && j <= nl) {
            if (cmp(win[i], lose[j])) a[++k] = win[i++];
            else                      a[++k] = lose[j++];
        }
        while (i <= nw) a[++k] = win[i++];
        while (j <= nl) a[++k] = lose[j++];
    }

    cout << a[q].id << '\n';
    return 0;
}
