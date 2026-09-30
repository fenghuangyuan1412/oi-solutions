#include <bits/stdc++.h>
using namespace std;

// 国籍 x_{i,j} <= 10^5，所以计数数组开到 10^5 + 5 就够。
// 用普通数组而不是 map：下标直接就是国籍号，单次读写 O(1)，
// 且全局只有一份，不需要为每艘船重建。
const int MAXX = 100000 + 5;
int cnt[MAXX];            // cnt[c] = 当前窗口内国籍为 c 的乘客人头数

// 队列里存的不是"船"，而是"乘客"：(国籍, 该乘客所属船的到达时间)。
// 存乘客而不是存船，是因为窗口边界按时间切，可能切在一艘船的中间吗？
// 不会——同一艘船的乘客时间全相同，要么整批在窗口内要么整批过期；
// 但按乘客存能让"进出各一次"的均摊分析最干净，代码也最简单。
struct Passenger {
    int country;
    int t;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    queue<Passenger> q;   // 队头 = 最早到达的乘客，天然按时间非降排列
    int distinct = 0;     // 窗口内出现过至少一个人的国籍种数，即当前答案

    for (int i = 1; i <= n; ++i) {
        int t, k;
        cin >> t >> k;

        // 第 1 步：本船乘客全部进窗口。
        // t > t-86400 恒成立，所以本船乘客一定满足窗口条件，先进来不会错。
        for (int j = 0; j < k; ++j) {
            int x;
            cin >> x;
            // 关键增量维护：只有 0 -> 1 的跳变才代表"多了一个国家"。
            // 同船同国籍的第二个人只会让 cnt++ ，distinct 不动。
            if (cnt[x]++ == 0) ++distinct;
            q.push({x, t});
        }

        // 第 2 步：把过期的乘客从队头踢出去。
        // 窗口是左开右闭 (t-86400, t]，所以"等于左边界"也过期，判定用 <=。
        // 必须用 while 而不是 if：时间跨度大时一次可能踢掉好几艘船的乘客。
        const int limit = t - 86400;
        while (!q.empty() && q.front().t <= limit) {
            const Passenger p = q.front();
            q.pop();
            // 对称地回退：只有 1 -> 0 的跳变才代表"少了一个国家"。
            if (--cnt[p.country] == 0) --distinct;
        }
        // 由队列的单调性（按船序入队、同船时间相同），队头一合格，
        // 后面的人时间更大必然都合格，所以不用往队尾方向找。

        // 第 3 步：输出本船的答案。每艘船一行，不是只输出最后一艘。
        cout << distinct << '\n';
    }
    return 0;
}
