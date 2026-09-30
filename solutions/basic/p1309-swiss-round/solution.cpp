#include <bits/stdc++.h>
using namespace std;

const int N = 200005;

struct Player {
    int id;         // 编号（1 基）
    int s;          // 总分
    int w;          // 实力值
};

Player p[N];
int n, r, q;

// 排名规则：总分高者靠前；同分则编号小者靠前（题面原话："总分相同的，约定编号较小的选手排名靠前"）
bool cmp(const Player& x, const Player& y) {
    if (x.s != y.s) return x.s > y.s;
    return x.id < y.id;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> r >> q;
    for (int i = 1; i <= 2 * n; i++) { cin >> p[i].s; p[i].id = i; }
    for (int i = 1; i <= 2 * n; i++) cin >> p[i].w;

    sort(p + 1, p + 2 * n + 1, cmp);          // 第一轮开始前先按初始分排好

    for (int round = 1; round <= r; round++) {
        // 相邻两人一对：(1,2) (3,4) …… 实力值大者得分（数据保证实力值两两不同）
        for (int i = 1; i <= 2 * n; i += 2) {
            if (p[i].w > p[i + 1].w) p[i].s += 1;
            else                     p[i + 1].s += 1;
        }
        sort(p + 1, p + 2 * n + 1, cmp);      // ← 每轮重新排序：O(R·N logN)，本题的"暴力档"
    }

    cout << p[q].id << '\n';
    return 0;
}
