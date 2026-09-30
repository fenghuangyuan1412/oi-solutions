#include <bits/stdc++.h>
using namespace std;

// 满分做法：枚举"一共闪了几次"，把休息秒数直接算出来，不再逐秒模拟
//
// 规则：跑 1 秒 17m；闪 1 秒 60m 并消耗 10 点魔法；原地休息 1 秒恢复 4 点魔法（只有休息才恢复）。
// 设一共闪 k 次：
//   需要魔法 10k，初始 M，缺口靠休息补 ⇒ rest(k) = ceil(max(0, 10k - M) / 4) 秒
//   这 k 次闪 + rest 次休息共花 k + rest(k) 秒，推进 60k 米
//   剩下的秒数全部用来跑，每秒 17 米
// 于是"闪 k 次"时，走完 S 米所需的最少秒数：
//   t(k) = k + rest(k) + ceil( max(0, S - 60k) / 17 )
// 答案 = min over k 的 t(k)，再和 T 比。
//
// 为什么可以这样"先算总量再排顺序"？因为总距离只跟各动作的秒数有关，
// 而休息随时可以插在需要魔法的那一闪之前，魔法上限 M 不构成额外限制（缺多少补多少）。
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long M, S, T;
    cin >> M >> S >> T;

    long long best = (long long)4e18;                       // 逃离所需的最短时间，先设成"无穷大"

    // k 的上界：再多闪也没意义 —— 一是魔法攒不出来（T 秒全休息也就 4T 点），二是 60k 超过 S 就够远了
    long long kmax = (M + 4 * T) / 10 + 2;
    for (long long k = 0; k <= kmax; k++) {
        long long lack = 10 * k - M;                         // 魔法缺口
        long long rest = lack > 0 ? (lack + 3) / 4 : 0;      // ceil(lack/4)，注意整数向上取整的写法
        if (k + rest > T) break;                             // 光闪加休息就超时，后面 k 更大只会更糟
        long long left = S - 60 * k;                         // 闪完之后还差多少米
        long long run = left > 0 ? (left + 16) / 17 : 0;     // ceil(left/17) 秒跑步补齐
        best = min(best, k + rest + run);
    }

    if (best <= T) {
        cout << "Yes\n" << best << '\n';
    } else {
        // 逃不掉：输出 T 秒内能走的最远距离（同样是枚举 k 取最大，注意 k+rest<=T 才合法）
        long long dist = 0;
        for (long long k = 0; k <= kmax; k++) {
            long long lack = 10 * k - M;
            long long rest = lack > 0 ? (lack + 3) / 4 : 0;
            if (k + rest > T) break;
            dist = max(dist, 60 * k + 17 * (T - k - rest));
        }
        cout << "No\n" << dist << '\n';
    }
    return 0;
}
