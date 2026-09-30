// P3613 【深基15.例2】寄包柜
// 编译：g++ -static -O2 -std=c++14 solution.cpp -o sol.exe
//
// 模型：稀疏存储。整柜子的格子总数按值域算是 n * max(a_i) = 10^5 * 10^5 = 10^10 格，
// 但真正被操作"点名"过的格子最多 q = 10^5 个 —— 所以不存整张表，只存出现过的键。

#include <cstdio>
#include <map>
using namespace std;

typedef long long ll;

// 键编码：把二维的 (柜子号 i, 格子号 j) 压成一个整数键。
// 乘数 B 必须严格大于 j 的取值跨度，否则不同二元组会撞进同一个键。
// j 最大 10^5，所以取 B = 10^5 + 1 = 100001，留出一格余量，绝不"踩线"。
const ll B = 100001;

inline ll key(int i, int j) {
    return (ll)i * B + j;   // 必须先转 ll 再乘：i*B 最大约 10^10，int 存不下
}

int main() {
    int n, q;
    if (scanf("%d%d", &n, &q) != 2) return 0;
    // n 读进来但用不上，这是故意的：题面明说"我们并不知道各个 a_i 的值"，
    // 而稀疏存储根本不需要知道每个柜子有几格 —— 这正是本题的突破口。
    (void)n;

    map<ll, int> mp;   // 键 -> 该格子当前的物品编号；不在表里 == 格子是空的

    for (int t = 0; t < q; ++t) {
        int op, i, j;
        scanf("%d%d%d", &op, &i, &j);

        if (op == 1) {
            int k;
            scanf("%d", &k);
            if (k == 0) {
                // k = 0 不是"存入编号为 0 的物品"，而是【清空】这一格。
                // 这里选择 erase：让哈希表保持"只存真正有东西的格子"，
                // 规模始终 <= 10^5，查询未命中时的判断也最干净。
                mp.erase(key(i, j));
            } else {
                mp[key(i, j)] = k;   // 存入直接覆盖旧值，与真实柜子的语义一致
            }
        } else {
            // 查询：题面只保证"查询的柜子有存过东西"，
            // 并不保证这个格子有东西 —— 它可能从没存过，也可能被 k=0 清空过。
            // 两种情况都必须输出 0，而不是"不输出"。
            map<ll, int>::iterator it = mp.find(key(i, j));
            printf("%d\n", it == mp.end() ? 0 : it->second);
        }
    }
    return 0;
}
