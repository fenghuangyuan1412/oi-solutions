#include <bits/stdc++.h>
using namespace std;

// P2234 [HNOI2002] 营业额统计
// 正解：有序集合 std::set（红黑树）查前驱/后继，整体 O(n log n)。
// 编译：g++ -static -O2 -std=c++14 solution.cpp -o sol.exe
int main() {
    // 关掉 cin 与 stdio 的同步、取消读前刷新：n 可达 32767，
    // 默认同步不影响正确性，但会把读入拖慢一个量级（这里只是求稳，不是决定性的）。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;   // 空输入直接退出，避免后面读未初始化的 n

    // s 里始终保存"第 i 天之前（含第 1 天）已经出现过的营业额"，去重即可：
    // 重复值只可能让差变成 0，而去掉重复并不会丢掉"差为 0"这个事实本身
    // （集合里还留着那一份，lower_bound 命中相等元素时差就是 0）。
    set<int> s;

    // 结果虽然题目保证 < 2^31，但中间量（尤其若干 2*10^6 的差相加）逼近 int 边界，
    // 用 long long 装 ans 是零成本的保险。
    long long ans = 0;

    for (int i = 1; i <= n; ++i) {
        int a;
        cin >> a;

        if (i == 1) {
            // 题面的特殊规定：第一天的最小波动值就是第一天的营业额本身，
            // 不是 0、也不是 |a1|。这是"题目硬性定义的边界"，不是数学推导的结果，
            // 所以 a1 为负时按字面就会加进一个负数。
            ans = a;
        } else {
            // lower_bound(a) 返回第一个 >= a 的元素（后继 R）；
            // 它的前一个元素就是最后一个 <= a 的元素（前驱 L）。
            // 观察：|a - x| 最小时 x 只能是 L 或 R，其余已出现的数都离得更远。
            set<int>::iterator it = s.lower_bound(a);

            // best 记录两个候选里更小的差。集合此时非空（第 1 天已插入），
            // 所以 L、R 至少存在一个，best 一定会被更新，不会是 INT_MAX。
            int best = INT_MAX;

            if (it != s.end()) {
                // R 存在：差是 *it - a（>= 0）。若 *it == a，这里得 0，答案当场最优。
                best = min(best, *it - a);
            }
            if (it != s.begin()) {
                // 只有在"it 不是第一个元素"时 prev(it) 才有意义。
                // 注意两个必须拦住的非法情形：
                //   it == begin() 时 prev(it) 是未定义行为（begin() 没有前驱）；
                //   it == end()   时 *it 是未定义行为（end() 不可解引用），
                // 但 end() 的 prev 是合法的（集合非空），所以上面的 end() 判断只管 *it。
                best = min(best, a - *prev(it));
            }

            ans += best;
        }

        // 插回集合：第 i+1 天要能看见第 i 天的营业额。
        // 顺序很重要 —— 必须是"先查再插"，否则查到的可能是自己（差恒为 0），
        // 把整道题退化成答案 0。
        s.insert(a);
    }

    cout << ans << '\n';   // 用 '\n' 而不是 endl：endl 会额外 flush
    return 0;
}
