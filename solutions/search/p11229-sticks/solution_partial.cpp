// P11229 [CSP-J 2024] 小木棍 —— 骗分版：DFS 回溯（只做"下界"剪枝）
// 思路：枚举位数 L=1,2,3…；对每个 L 从最高位往低位列举数字（0~9 升序），
//       唯一剪枝：后面每个空位至少还要 2 根，已用根数 + 2×剩余空位 > n 就回头。
// 实测：n≤50 很快（详见 README）；n=65 起 3 秒级，n=70 实测 20 秒跑不完。
// 编译：g++ -static -O2 -std=c++14 solution_partial.cpp -o <输出>
#include <bits/stdc++.h>
using namespace std;

const int C[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

int n, L, dig[50005], sol[50005];
long long nodes;   // 只给讲解用：搜索过程中访问过的结点总数（打 stderr）

bool dfs(int depth, int used) {
    ++nodes;
    if (depth == L) return used == n;              // 到最后一位：必须恰好用完
    if (used + 2 * (L - depth) > n) return false;  // 剪枝①：全放"1"也嫌多
    // 注意：故意没写"全放 8 也不够"的剪枝 used+7*(L-depth)<n —— 那是正解的一半
    for (int d = (depth == 0 ? 1 : 0); d <= 9; ++d) {
        dig[depth] = d;
        if (dfs(depth + 1, used + C[d])) { sol[depth] = d; return true; }
    }
    return false;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        scanf("%d", &n);
        nodes = 0;
        if (n == 1) { puts("-1"); continue; }
        bool ok = false;
        for (L = 1; 2 * L <= n; ++L) {     // 位数从小到大 ⇒ 第一个解就是最小的数
            if (7 * L < n) continue;        // 位数太少，全放 8 也凑不够 n 根
            if (dfs(0, 0)) { ok = true; break; }
        }
        if (!ok) { puts("-1"); continue; }
        for (int i = 0; i < L; ++i) printf("%d", sol[i]);
        putchar('\n');
        fprintf(stderr, "n=%d nodes=%lld\n", n, nodes);   // 讲解用，评测端可忽略
    }
    return 0;
}
