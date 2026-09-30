# OI 题解知识库

信息学奥赛（CSP / NOIP / 洛谷 / Codeforces）题解仓库。每道题一个目录，包含题意压缩、思路推导、正确性论证、逐段讲解代码和可视化演示。

- 目录结构与写作规范见 [notes/CONVENTIONS.md](notes/CONVENTIONS.md)
- 新建题目：`./scripts/new_problem.sh P1048 dp/knapsack 采药`

## 题库索引

索引由题解目录自动汇总，新增题目后更新此处。

| 主题 | 题数 | 目录 |
| --- | --- | --- |
| 基础 | 1 | [solutions/basic](solutions/basic) |
| 动态规划 | 1 | [solutions/dp](solutions/dp) |
| 数据结构 | 0 | [solutions/ds](solutions/ds) |
| 图论 | 0 | [solutions/graph](solutions/graph) |
| 搜索 | 0 | [solutions/search](solutions/search) |
| 贪心 | 2 | [solutions/greedy](solutions/greedy) |
| 数学 | 0 | [solutions/math](solutions/math) |
| 字符串 | 0 | [solutions/string](solutions/string) |
| 计算几何 | 0 | [solutions/geometry](solutions/geometry) |

### 题目列表

| 题号 | 题目 | 主题 | 核心思想 | 可视化 |
| --- | --- | --- | --- | --- |
| [P14357](https://www.luogu.com.cn/problem/P14357) | [CSP-J 2025] 拼数 | 贪心 / 计数排序 | 位数用满 + 降序交换论证 | [html](solutions/greedy/p14357-number/visualization.html) |
| [P14358](https://www.luogu.com.cn/problem/P14358) | [CSP-J 2025] 座位 | 基础 / 模拟·排序 | 名次 k → 列 ceil(k/n)，行按列号奇偶翻转 | [html](solutions/basic/p14358-seat/visualization.html) |
| [P14359](https://www.luogu.com.cn/problem/P14359) | [CSP-J 2025] 异或和 | 贪心 / 前缀异或 | 前缀异或配对 + 最多不相交区间最早结束贪心 | [html](solutions/greedy/p14359-xor/visualization.html) |
| [P14360](https://www.luogu.com.cn/problem/P14360) | [CSP-J 2025] 多边形 | 动态规划 / 背包计数 | 极值锚定 + 补集转化，m≥3 由判据自动蕴含 | [html](solutions/dp/p14360-polygon/visualization.html) |

## 单题格式

```
solutions/basic/p14358-seat/
├── README.md          # 题解：题意 → 暴力 → 观察 → 算法 → 正确性 → 复杂度
├── solution.cpp       # 带逐段注释的代码
├── metadata.yml       # 难度、标签、复杂度、评测状态
├── verify.js          # 可选，与暴力实现随机对拍的验证脚本
└── visualization.html # 可选，浏览器直接打开的分步动画
```

GitHub 原生渲染 Markdown 中的 Mermaid 流程图与 `$$...$$` 公式，因此题解正文在仓库页面内即可直接看图，无需额外构建。

## 讲义索引（notes/）

跨题的整套讲评/教案，面向"要给别人上课"的场景。每份讲义旁边配了**单文件、内联 CSS/JS、不联网**的分步可视化，双击即可在浏览器播放。

| 讲义 | 覆盖 | 可视化 |
| --- | --- | --- |
| ★ [notes/cspj-trend-2021-2025.md](notes/cspj-trend-2021-2025.md) | **CSP-J 第二轮考情趋势分析（2021—2025）**：五年逐题表 + 算法考核清单（带优先级）+ T1—T4 位置指纹 + 16 课时排课建议。重点核对 2023/2024/2025 | —— |
| ★ [notes/mock-cspj-round2/](notes/mock-cspj-round2) | **自编 CSP-J 第二轮模拟卷 4 套 × 4 题（不是任何一年真题）**，按上面的趋势逐题仿出：`paper.html` 是可截图的仿真题面，每题另有题解 + 分步可视化，答案在 `answer-key.md` | 16 个（每题一个 `visualization.html`），题面排版另有 `sheetshots.js` 逐页截图核对 |
| [notes/luogu-2026-j1/](notes/luogu-2026-j1) | **2026 第一轮 · 洛谷 SCP-J1 卷（洛谷命制，不是 CCF 真题）**，42 题零基础讲评，题面全文见 `paper.md` | 孪生素数、二维 dp 填表、九连环 dfs、二分第 k 小、分层 BFS 迷宫（共 5 个，见讲义 §0.1） |
| [notes/luogu-2026-s1/](notes/luogu-2026-s1) | **2026 第一轮 · 洛谷 SCP-S1 卷（洛谷命制，不是 CCF 真题）**，43 题零基础讲评，题面全文见 `paper.md` | permanent 状压 dp、Fibonacci 词 + Zeckendorf、异或哈希必经边（共 3 个，见讲义 §0.1） |
| [notes/mock-cspjs-round1/](notes/mock-cspjs-round1) | 自编仿真题集（**不是任何一份真题**，按官方题型结构 100 分出的练习） | [j-trace.html](notes/mock-cspjs-round1/j-trace.html)、[s-trace.html](notes/mock-cspjs-round1/s-trace.html) |

## 参考项目

本仓库的组织方式参考了以下开源项目：

- [OI-wiki](https://github.com/OI-wiki/OI-wiki) — 主题分类体系（`dp` / `ds` / `graph` / `string` …）
- [a1fredbao/OI-Solutions](https://github.com/a1fredbao/OI-Solutions) — 按平台分目录的题解记录格式
- [EndlessCheng/codeforces-go](https://github.com/EndlessCheng/codeforces-go) — 按专题归纳的题解与模板
- [cp-algorithms](https://github.com/cp-algorithms/cp-algorithms) — 算法原理讲解结构
