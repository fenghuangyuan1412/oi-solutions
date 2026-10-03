#include <bits/stdc++.h>
using namespace std;

/*
 * P1616 疯狂的采药 —— 完全背包
 *
 * 与 P1048 采药（01 背包）相比，题面只多了一句话：
 *   "每种草药可以无限制地疯狂采摘。"
 * 代码上也只差一个方向：内层枚举容量时，P1048 写【倒序】，本题写【正序】。
 *
 * 为什么正序 == 无限次？
 *   本轮 dp[j] = max(dp[j], dp[j-a] + b)，其中 dp[j-a] 因为正序（j-a < j 且先被算）
 *   已经在本轮更新过，它可能已经包含了一株第 i 种草药；于是 +b 就表示"再采一株"。
 *   把这一步不断展开可得
 *       dp[j] = max_{k >= 0, k*a <= j} ( dp_上一轮[j - k*a] + k*b )，
 *   k 正是第 i 种草药采摘的株数 —— 这就把"无限次"编码进了转移。
 *   （完整归纳证明见 README「正确性说明」）
 *
 * 复杂度：O(m * t)。题面保证 m * t <= 1e7，所以朴素完全背包绰绰有余。
 *
 * 数值：最坏 t/a_min * b_max = 1e7 * 1e4 = 1e11，int（约 2.14e9）装不下，必须 long long。
 *       10^7+5 个 long long 约 80 MB，必须开在全局区，放 main 里会爆栈。
 */

const int MAXT = 10000005;  // t <= 1e7
long long dp[MAXT];         // dp[j] = 用不超过 j 时间能采到的最大价值（一维滚动）

int main() {
  int t, m;
  if (scanf("%d%d", &t, &m) != 2) return 0;  // 注意：先总时间 t，后草药种数 m

  for (int i = 1; i <= m; ++i) {
    int a, b;
    scanf("%d%d", &a, &b);                  // a = 采摘耗时，b = 价值
    for (int j = a; j <= t; ++j)            // 【正序】—— 完全背包的唯一标志
      dp[j] = max(dp[j], dp[j - a] + b);    // j < a 时装不下，保持原值，故从 a 起
  }

  printf("%lld\n", dp[t]);
  return 0;
}
