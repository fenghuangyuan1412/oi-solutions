#include <bits/stdc++.h>
using namespace std;

/*
 * P1352 没有上司的舞会 —— 树形 DP 入门
 *
 * 模型：一棵以校长为根的树。选一个点获得 r_u 的快乐值，但【父子不能同时选】，
 *       求能选出的最大快乐值之和。等价于"树上最大权独立集"。
 *
 * 状态：对每个节点 u 只有"来 / 不来"两种可能，所以
 *       f[u][0] = u 不来时，以 u 为根的子树能拿到的最大快乐值
 *       f[u][1] = u 来  时，以 u 为根的子树能拿到的最大快乐值
 *
 * 转移（把子树的最优值"合并"上来，这就是"树形 DP"名字的由来）：
 *       f[u][1] = r_u + Σ_子节点v f[v][0]          // u 来了，孩子们都不能来
 *       f[u][0] =     Σ_子节点v max(f[v][0], f[v][1]) // u 没来，孩子们来不来都行，各取更优
 *
 * 答案：max(f[root][0], f[root][1])。
 *
 * 计算顺序：必须是【后序遍历】——先算完所有孩子，才能合并出 u 的值。
 *
 * 为什么全负数据答案也是 0 而不是负数？
 *   f[u][0] = Σ max(f[v][0], f[v][1]) >= 0（叶子 f[0]=0，归纳下去每一项都非负），
 *   所以根节点的 f[root][0] 永远 >= 0，代表"这一棵子树一个人都不请"。
 *
 * 复杂度：每个节点访问一次 ⇒ O(N) 时间、O(N) 空间（N <= 6000）。
 */

const int MAXN = 6005;

int n;
int r_[MAXN];              // 快乐指数，注意可以是负数（-128 <= r_i <= 127）
vector<int> child_[MAXN];  // 孩子表
bool hasParent[MAXN];      // 用来找根：没有父亲的节点就是校长
int f[MAXN][2];

void dfs(int u) {
  f[u][1] = r_[u];         // u 来：至少拿到自己的快乐值
  f[u][0] = 0;             // u 不来：初值 0，下面把孩子们的最优值累加进来
  for (int v : child_[u]) {
    dfs(v);                // 先把孩子算完（后序）
    f[u][1] += f[v][0];                    // u 来了 -> 孩子 v 一定不能来
    f[u][0] += max(f[v][0], f[v][1]);      // u 没来 -> 孩子 v 来不来都行，取更优
  }
}

int main() {
  if (scanf("%d", &n) != 1) return 0;

  for (int i = 1; i <= n; ++i) scanf("%d", &r_[i]);

  for (int i = 1; i < n; ++i) {            // 共 N-1 条边
    int L, K;
    scanf("%d%d", &L, &K);                 // K 是 L 的直接上司，即边 K -> L
    child_[K].push_back(L);
    hasParent[L] = true;
  }

  int root = 1;                            // 校长：唯一没有上司的人
  for (int i = 1; i <= n; ++i)
    if (!hasParent[i]) { root = i; break; }

  dfs(root);
  printf("%d\n", max(f[root][0], f[root][1]));
  return 0;
}
