#include <bits/stdc++.h>
using namespace std;

// 这是"考场上只来得及写 20 分钟"的版本：逐秒模拟，能闪就闪，闪不动就跑，**永远不休息**。
//
// 实测结论（本机 g++ -static -O2）：
//   样例 1  39 200 4   -> No / 197   ✅ 与官方输出一致
//   样例 2  36 255 10  -> Yes / 8    ❌ 官方是 Yes / 6
// 也就是说这份代码连样例都过不了，只能靠"魔法本来就不够用、休息没意义"的那批点拿分。
// 它的价值是当**反面教材**：漏掉"休息换魔法"这条规则，贪心就只是局部最优。
// 拿满分的写法见同目录 solution.cpp（枚举闪光次数，把休息秒数算出来）。
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long M, S, T;
    cin >> M >> S >> T;

    long long dist = 0;
    long long ans = -1;
    for (long long t = 1; t <= T; t++) {
        if (M >= 10) { dist += 60; M -= 10; }   // 能闪就闪：一秒 60m > 一秒 17m
        else         { dist += 17; }            // 闪不动就跑步，从没想到"这一秒什么都不走攒魔法"更划算
        if (dist >= S && ans < 0) ans = t;
    }

    if (ans > 0) cout << "Yes\n" << ans << '\n';
    else         cout << "No\n" << dist << '\n';
    return 0;
}
