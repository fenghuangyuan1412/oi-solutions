// P1752 点菜 —— 二分答案 + 价格大根堆贪心
// 编译：g++ -static -O2 -std=c++14 solution.cpp -o sol.exe
//
// 题意：n 个人每周来一次、每人每周至多点一道菜；p 个挑剔的人只能点"美味度 >= 下限"的菜，
//       q 个贫穷的人只能点"价格 <= 上限"的菜（两类人不重叠），剩下 n-p-q 个普通人随便点。
//       求把 m 道菜全部点过一遍最少要几周，无解输出 -1。
//
// 主思路（见 README）：答案对周数单调（week 周行得通，week+1 周只会更宽裕），二分周数；
//       判定 week 周是否够用时，用"分配视角"代替"逐周模拟"：
//       每人一周至多一道 => 每人 week 周至多 week 道，于是"排周次"根本不重要，
//       只需要判断能否把 m 道菜分给 n 个人、每人不超过 week 道。

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int MAXM = 200005;
const int MAXN = 50005;

int n, m, p, q, z;          // z = n - p - q：既不挑剔也不贫穷的普通人（他们什么都吃）

struct Dish { ll del, pri; };   // 一道菜 = (美味度, 价格)，必须捆绑排序，绝不能拆成两个数组各排各的
Dish dish[MAXM];
ll pickyMin[MAXN];          // 挑剔的人能接受的美味度下限（从大到小排序）
ll poorMax[MAXN];           // 贫穷的人能点的价格上限（从大到小排序）

// 判定：week 周内能否把所有菜点过一遍。
// 堆里放的是"挑剔的人还有资格选、但还没被吃掉"的菜的价格（大根堆 = 堆顶最贵）。
bool check(ll week) {
    priority_queue<ll> heap;
    int idx = 0;                       // 指向下一道还没进堆的菜（美味度从大到小排好了）

    for (int i = 0; i < p; i++) {      // 挑剔的人从最挑剔到最不挑剔
        // 刚够第 i 个人吃的菜（美味度 >= 他的下限），此刻才有资格进堆
        while (idx < m && dish[idx].del >= pickyMin[i])
            heap.push(dish[idx++].pri);
        // 他总共至多吃 week 道（每周一道）；吃堆里最贵的 week 道。
        // 为什么挑最贵的？贵菜穷人吃不起，只有挑剔/普通人能消化，
        // 让挑剔的人把贵菜"扛走"，给后面的人留下最便宜的菜。
        for (ll t = 0; t < week && !heap.empty(); t++)
            heap.pop();
    }

    // 美味度低于所有下限的菜，挑剔的人一个都不会吃，只能靠穷/普通人消化，也丢进堆
    while (idx < m)
        heap.push(dish[idx++].pri);

    // 贫穷的人从最有钱的开始。他吃不起的菜（价格 > 他的上限），比他更穷的人更吃不起，
    // 只能甩给普通人，先记账（forced）；吃得起的部分，照旧挑最贵的 week 道。
    ll forced = 0;
    for (int j = 0; j < q; j++) {
        while (!heap.empty() && heap.top() > poorMax[j]) {
            heap.pop();
            forced++;
        }
        for (ll t = 0; t < week && !heap.empty(); t++)
            heap.pop();
    }

    // 剩下的（穷人吃不起被甩掉的 + 穷人容量用完没吃掉的）全部由普通人消化。
    // 普通人 z 人、每人至多 week 道 => 容量 z*week。z*week 可达 1e10，必须 long long！
    return forced + (ll)heap.size() <= z * week;
}

int main() {
    scanf("%d%d%d%d", &n, &m, &p, &q);
    for (int i = 0; i < m; i++) scanf("%lld%lld", &dish[i].del, &dish[i].pri);
    for (int i = 0; i < p; i++) scanf("%lld", &pickyMin[i]);
    for (int i = 0; i < q; i++) scanf("%lld", &poorMax[i]);

    sort(dish, dish + m, [](const Dish &x, const Dish &y) { return x.del > y.del; }); // 菜按美味度从大到小
    sort(pickyMin, pickyMin + p, greater<ll>()); // 挑剔的人从最挑剔到最不挑剔
    sort(poorMax, poorMax + q, greater<ll>());   // 贫穷的人从最有钱到最没钱
    z = n - p - q;

    if (!check(m)) {                 // 人人都放开吃（每人可吃 m 道）还不够 => 永远无解
        printf("-1\n");
        return 0;
    }
    int lo = 1, hi = m;              // 至少要 1 周（m>=1），至多 m 周一定够
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (check(mid)) hi = mid;
        else lo = mid + 1;
    }
    printf("%d\n", lo);
    return 0;
}
