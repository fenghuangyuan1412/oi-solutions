# 第 2 套 · 参考答案与评分速查（教师版）

> **本文件是答案速查，只给老师**：学生拿到的题面（`problem.txt` / `paper.md` / `paper.html`）里没有这些内容，截图题面时千万不要把它拍进去。监考、批改时看这一页即可。
> 详细推导、正确性证明、可视化都在各题的 `README.md` 与 `visualization.html` 里，每题末尾给链接。
> 每题满分 100 分，全卷 400 分，认证时间 210 分钟。

## 一分钟速查

| 题 | 考点 | 一句话答案 | 时间复杂度 | 样例输出（本机实测） |
| --- | --- | --- | --- | --- |
| T1 角色图鉴 | 标记数组去重 | 数出现过几种角色，用 $m$ 减掉：$m - \text{distinct}$ | $O(n+m)$ | `5` / `0` |
| T2 谷仓机器人 | 撞墙右转的网格模拟 | 逐条照做指令，撞墙/货架只右转不移动，`bool seen` 判重 | $O(nm+k)$ | `1 2 2 3` / `3 2 3 4` |
| T3 应援灯牌 | 七段码贪心构造 | 位数压到 $\lceil n/7\rceil$，逐位取"能填满剩余 LED"的最小数字 | $O(n)$ | `2` / `10` / `-1` |
| T4 总选打投 | 分层计数 DP + 同值排除 | `dp[s][v]` 记末尾值，转移用 `tot[s-v] - dp[s-v][v]` 挖掉相邻同值 | $O(m\cdot S\cdot V)$ | `1` / `2` / `2` |

极限数据本机实测（`g++ -static -O2 -std=c++14`，同一台 Windows + MinGW 13.1，取三次最快）：

| 题 | 极限数据 | 输出 | 用时 |
| --- | --- | --- | --- |
| T1 | $n = m = 10^6$（$c_i$ 均匀随机） | `368468`（不同角色 631532 种） | 0.815 s |
| T2 | $n = m = 1000$，$k = 10^6$ | `275 436 1 74929` | 0.160 s |
| T3 | $n = 10^5$ | 14286 位，`2` 后跟 14285 个 `8` | 0.044 s |
| T4 | $m = 100$，$S = 5000$，$\sum k_i = 10^4$ | `704599713` | 0.215 s |

四题均已与随卷 `brute.cpp` 在本机随机对拍 **5000 组**（[`../verify/RESULTS.md`](../verify/RESULTS.md) 每题有一行），**0 处不一致**。另有两处写作时另跑的临时核对（不在自动记录里）：T3 按 $n = 1..150$ 逐个全量对过，T4 用一份结构完全不同的字典 DP 交叉核过。

---

## T1 角色图鉴 / codex

**核心思路一句话**：题面"运气最好"把过程抹平了，只剩一次减法——开 `bool have[m+1]`，一遍扫过序列，第一次见到才 `distinct++`，答案 `= m - distinct`。

核心代码（摘自 [`t1-codex/solution.cpp`](t1-codex/solution.cpp)）：

```cpp
bool have[M]; int distinct = 0;                    // M = 1000005，放全局自动清零
for (int i = 1; i <= n; i++) {
    int c; scanf("%d", &c);
    if (!have[c]) { have[c] = true; distinct++; }  // 灵魂：只有第一次见到才 +1
}
printf("%d\n", m - distinct);
```

**样例正确答案**（与 README 实测一致）：`5 8 / 1 2 2 3 1` → `5`；`3 3 / 1 2 3` → `0`。

**部分分怎么给**（子任务见题面表）：

| 子任务 | 分值 | 能过的做法 | 判分提示 |
| --- | --- | --- | --- |
| 1—3 | 15 | $n \le 10$：套 `set` 去重取 `size`，或 `sort+unique`（`brute.cpp`） | 数据太小，暴力也满分 |
| 4—8 | 25 | $c_i$ 两两不同：没有复数，直接输出 $m - n$ | 最白送的一档，连去重都不用真写 |
| 9—13 | 25 | $m \le 100$：`bool have[105]` 标记一遍即可 | 值域极小，标记数组最直观 |
| 14—20 | 35 | 完整标记数组 $O(n)$ | 见下方扣分点 |

**常见扣分点**：① 忘 `if(!have[c])` 判断 = 没去重，样例 1 会得 `8-5=3` 而不是 `5`；② 数组开在 `main` 里不清零，读到脏内存；③ 写成 `bool have[1000000]`，$c_i = 10^6$ 越界；④ 写反成 `n - distinct`；⑤ `cin` 未关同步，$10^6$ 次读入偏慢。

**详解**：[t1-codex/README.md](t1-codex/README.md) ｜ 演示：[t1-codex/visualization.html](t1-codex/visualization.html)

---

## T2 谷仓机器人 / robot

**核心思路一句话**：正解就是模拟——按 `d` 编码查方向数组，`F` 时目标格合法才前进、否则只把 `d=(d+1)&3` 右转不移动，`R` 也右转；用 `bool seen[n+1][m+1]` 把"是否走过"降到 $O(1)$，出发格先置 `true`、`cnt=1`。

核心代码（摘自 [`t2-robot/solution.cpp`](t2-robot/solution.cpp)）：

```cpp
int dr[4] = {0,1,0,-1}, dc[4] = {1,0,-1,0};       // 0 东 1 南 2 西 3 北
seen[r][c] = true; cnt = 1;                        // 出发格也算走过
for (int t = 0; t < k; t++) {
    if (op[t] == 'R') d = (d + 1) & 3;             // 原地右转
    else {
        int nr = r + dr[d], nc = c + dc[d];
        if (nr>=1 && nr<=n && nc>=1 && nc<=m && g[nr][nc] != 'x') {
            r = nr; c = nc;
            if (!seen[r][c]) { seen[r][c] = true; cnt++; }   // 新格子才 +1
        } else d = (d + 1) & 3;                    // 撞了：只转向不移动
    }
}
printf("%d %d %d %d\n", r, c, d, cnt);
```

**样例正确答案**（与 README 实测一致）：`3 3 5 / .../.x./.../ 1 1 0 / FFFRF` → `1 2 2 3`；`3 3 6 / ..x/.../x../ 1 1 0 / FFFFFF` → `3 2 3 4`。

**部分分怎么给**：

| 子任务 | 分值 | 能过的做法 | 判分提示 |
| --- | --- | --- | --- |
| 1—3 | 15 | $n = 1$：网格退化成一条横线，撞墙只可能是左右出界 | 正解代码本身就能过，此档不额外加分 |
| 4—7 | 20 | 无货架：合法性只剩边界判断，不必查 `g=='x'` | 仍要正确处理撞墙右转 |
| 8—11 | 20 | $k \le 1000$：判重用 `set<pair>`（`brute.cpp`）甚至线性 `vector` 都够 | 暴力 A 到 $k=10^6$ 才崩，此档能抢 |
| 12—14 | 15 | 只有 $F$：无 `R` 指令，撞墙右转是唯一转向来源 | 分支少一半，写不出完整状态机的能抢这 15 分 |
| 15—20 | 30 | 完整模拟 + `bool seen` $O(1)$ 判重 | 见下方扣分点 |

**常见扣分点**：① 漏掉出发格 `seen[r][c]=true; cnt=1`，样例 1 得 `2` 而不是 `3`；② 撞墙的 `else` 分支里误写 `r=nr; c=nc` → 穿墙出界；③ 方向数组与 `d` 编码对不上（本题"行 +1"是南，`d=1`）；④ 先读 `g[nr][nc]` 后判越界（`&&` 要短路，边界条件在前）；⑤ 地图 0/1 下标混用；⑥ 第四个数输出成步数而不是"不同格子数"。

**详解**：[t2-robot/README.md](t2-robot/README.md) ｜ 演示：[t2-robot/visualization.html](t2-robot/visualization.html)

---

## T3 应援灯牌 / light

**核心思路一句话**：无前导零时位数越少数越小，先把位数压到最少 $L=\lceil n/7\rceil$（一位最多烧 7 颗）；若 $2L>n$（一位最少 2 颗）输出 `-1`；否则从高位到低位逐位取"让剩余 LED 落进 $[2\cdot\text{left},7\cdot\text{left}]$"的最小数字，判据精确所以不用回溯。

核心代码（摘自 [`t3-light/solution.cpp`](t3-light/solution.cpp)）：

```cpp
int cost[10] = {6,2,5,5,4,5,6,3,7,6};
int L = (n + 6) / 7;                    // ceil(n/7)
if (2 * L > n) { puts("-1"); return 0; } // 装不下就无解
int rem = n;
for (int pos = 0; pos < L; pos++) {
    int left = L - pos - 1;             // 当前位之后还欠几格
    for (int d = (pos == 0 ? 1 : 0); d <= 9; d++) {   // 首位跳过 0
        int r2 = rem - cost[d];
        if (r2 >= 2*left && r2 <= 7*left) { putchar('0'+d); rem = r2; break; }
    }
}
putchar('\n');
```

**样例正确答案**（与 README 实测一致）：`5` → `2`；`8` → `10`；`1` → `-1`。

**部分分怎么给**：

| 子任务 | 分值 | 能过的做法 | 判分提示 |
| --- | --- | --- | --- |
| 1—4 | 20 | $n \le 10$：位数最多 2，暴力 DP（`brute.cpp`）或直接枚举小整数核对 LED | 此档 `string g[]` DP 内存无压力 |
| 5—9 | 25 | 答案只有一位数：在 $d=1..9$ 里取 `cost[d]==n` 的最小 $d$，找不到 `-1` | 只查表不构造多位，很干净的抢分档 |
| 10—14 | 25 | $n \le 100$：暴力 B 的 DP 在这一档跑得动 | 满分解同一份贪心也过（$n=10^5$ 时它内存 705 MB 会挂） |
| 15—20 | 30 | 贪心 $L=\lceil n/7\rceil$ + 逐位判据 | 见下方扣分点 |

**常见扣分点**：① `ceil` 写成 `n/7`（漏 +6）少算一位；② 判据的 `left` 写成 `L-pos`，样例 2 会输出 `11` 而不是 `10`；③ 首位允许 0（`for(d=0...)`），样例 2 输出 `01`；④ 用整数存答案，$n=10^5$ 时 14286 位直接爆 `long long`，必须边算边 `putchar`；⑤ 特判 `n==1` 而非通用 `2*L>n`；⑥ 漏结尾换行。

**详解**：[t3-light/README.md](t3-light/README.md) ｜ 演示：[t3-light/visualization.html](t3-light/visualization.html)

---

## T4 总选打投 / vote

**核心思路一句话**：只记 `dp[s]` 判不了"相邻不许同值"，给状态加一维末尾值 `dp[s][v]`；再维护 `tot[s]=Σ_v dp[s][v]`，转移写成 `ndp[s][v] = cnt[v] × (tot[s-v] − dp[s-v][v])`，用"总数减掉昨天也是 `v` 那类"把 $O(V)$ 枚举压成 $O(1)$；第一天无相邻限制，`dp[v][v]=cnt[v]`。

核心代码（摘自 [`t4-vote/solution.cpp`](t4-vote/solution.cpp)）：

```cpp
if (i == 1) { for (int v = 1; v < V; v++) dp[v][v] = cnt[v]; }   // 第一天无相邻限制
else {
    memset(ndp, 0, sizeof ndp);
    for (int v = 1; v < V; v++) if (cnt[v]) {
        for (int s = v; s <= S; s++) {
            long long ways = tot[s - v] - dp[s - v][v];  // 挖掉"昨天也是 v"
            if (ways < 0) ways += MOD;                   // 余数相减可能为负
            ndp[s][v] = ways * cnt[v] % MOD;             // ×cnt[v]：同值多个项目
        }
    }
    memcpy(dp, ndp, sizeof dp);                          // 滚动到下一层
}
// 每天重建 tot[s]=Σ_v dp[s][v]；答案 = tot[S]
```

**样例正确答案**（与 README 实测一致）：`3 6 / 2 1 2 / 2 2 3 / 1 3` → `1`；`2 4 / 2 1 3 / 2 1 3` → `2`；`1 5 / 3 5 5 2` → `2`。

**部分分怎么给**：

| 子任务 | 分值 | 能过的做法 | 判分提示 |
| --- | --- | --- | --- |
| 1—3 | 15 | $m \le 8, S \le 50$：DFS 暴力（`brute.cpp`，`a[15][105]`）带 `sum>S` 剪枝 | 这一档暴力足够 |
| 4—7 | 20 | $k_i = 1$：每天唯一值，方案数非 0 即 1，累加并查相邻两天不同即可 | 线性扫描的抢分档，不必 DP |
| 8—11 | 20 | 同一天热度两两不同：`cnt[v]∈{0,1}` 可省 `×cnt[v]`，但二维 + 排除仍要写对 | 退化版，仍需 `dp[s][v]` |
| 12—15 | 15 | $m \le 20, S \le 500$：完整分层 DP 在小规模即拿分 | `brute.cpp` 数组只到第 14 天，此档要靠 DP |
| 16—20 | 30 | 完整 `tot - dp` 排除 + 滚动数组 | 见下方扣分点 |

**常见扣分点**：① **漏 `- dp[s-v][v]`**——样例 1、2 都查不出这个 bug（去掉减项仍输出 `1`、`2`），能查出的最小数据是 `2 2 / 1 1 / 1 1`（正解 `0`，漏减得 `1`），最大数据正解 `704599713`、漏减成 `16141876`；② 忘 `ways<0 时 +MOD`，负数取模会得到负答案（最大数据约 22.6% 转移触发）；③ 二维数组开在 `main` 里爆栈（`dp`/`ndp` 各约 2 MB）；④ 第二维按 `S` 开成 `dp[5005][5005]` = 400 MB；⑤ 同一天重复值用 `set/bool` 去重，样例 3 从 `2` 变 `1`；⑥ `ways*cnt[v]` 用 `int` 溢出（约 $10^{11}$），`ways` 必须 `long long`。

**详解**：[t4-vote/README.md](t4-vote/README.md) ｜ 演示：[t4-vote/visualization.html](t4-vote/visualization.html)

---

## 批改时的三个通用提醒

1. **对齐关系**：本套 4 题分别对齐 CSP-J 2024 第二轮 T1—T4（P11227 扑克牌 / P11228 地图探险 / P11229 小木棍 / P11230 接龙）的考点，但**都是自编模拟卷，不是任何一年的真题原题**。学生若整段背真题代码，T1（去重减 $m$ 而非别的口径）、T2（行 +1 是南的方向编码）、T3（拼最小需先压位数）、T4（相邻排除要减同值项）四处会直接错。
2. **`brute.cpp` 只用于对拍**：T3 的 `brute.cpp` 是 `string g[100005]` 的 DP，$n$ 大时内存 705 MB 会挂；T4 的 `brute.cpp` 数组只到 `a[15][105]`（第 14 天封顶）。它们本身是"部分分实现"，别当满分参考。
3. **本卷按标准输入输出练习**：正式复赛要求文件读写（`freopen`），学生若用 `cin/cout` 未关同步，T1/T2 的 $10^6$ 规模可能卡时限，批改时按"实现细节"提醒而不必判死。

## 各题详解与可视化

- T1 角色图鉴：[README](t1-codex/README.md) ｜ [visualization.html](t1-codex/visualization.html)
- T2 谷仓机器人：[README](t2-robot/README.md) ｜ [visualization.html](t2-robot/visualization.html)
- T3 应援灯牌：[README](t3-light/README.md) ｜ [visualization.html](t3-light/visualization.html)
- T4 总选打投：[README](t4-vote/README.md) ｜ [visualization.html](t4-vote/visualization.html)
