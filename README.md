# OI 题解知识库

信息学奥赛（CSP / NOIP / 洛谷 / Codeforces）题解仓库。每道题一个目录，包含题意压缩、思路推导、正确性论证、逐段讲解代码和可视化演示。

- 目录结构与写作规范见 [notes/CONVENTIONS.md](notes/CONVENTIONS.md)
- 新建题目：`./scripts/new_problem.sh P1048 dp/knapsack 采药`

## 题库索引

索引由题解目录自动汇总，新增题目后更新此处。

| 主题 | 题数 | 目录 |
| --- | --- | --- |
| 基础 | 10 | [solutions/basic](solutions/basic) |
| 动态规划 | 5 | [solutions/dp](solutions/dp) |
| 数据结构 | 8 | [solutions/ds](solutions/ds) |
| 图论 | 0 | [solutions/graph](solutions/graph) |
| 搜索 | 0 | [solutions/search](solutions/search) |
| 贪心 | 4 | [solutions/greedy](solutions/greedy) |
| 数学 | 0 | [solutions/math](solutions/math) |
| 字符串 | 0 | [solutions/string](solutions/string) |
| 计算几何 | 0 | [solutions/geometry](solutions/geometry) |

共 27 题（按"有 README.md 的目录"统计），全部来自训练单与洛谷/NOIP 真题，暂无半成品目录。

### 题目列表

| 题号 | 题目 | 主题 | 核心思想 | 可视化 |
| --- | --- | --- | --- | --- |
| [P1003](https://www.luogu.com.cn/problem/P1003) | [NOIP 2011 提高组] 铺地毯 | 基础 / 模拟·枚举 | 后铺的在上 ⇒ 倒序枚举、第一个命中就 `break`；真涂满 $10^5\times10^5$ 要 40 GB | —— |
| [P1309](https://www.luogu.com.cn/problem/P1309) | [NOIP 2011 普及组] 瑞士轮 | 基础 / 排序·模拟 | **骗分练习 ①**：暴力每轮 `sort` 本身就值分；正解靠"胜者/负者两队列各自天然有序"一次归并 | —— |
| [P14358](https://www.luogu.com.cn/problem/P14358) | [CSP-J 2025] 座位 | 基础 / 模拟·排序 | 名次 $k$ → 列 $\lceil k/n\rceil$，行按列号奇偶翻转 | [html](solutions/basic/p14358-seat/visualization.html) |
| [P1540](https://www.luogu.com.cn/problem/P1540) | [NOIP 2010 提高组] 机器翻译 | 基础 / 模拟·队列 | 淘汰规则是"最早进入"（FIFO）不是 LRU；命中时内存内容完全不动 | —— |
| [P1563](https://www.luogu.com.cn/problem/P1563) | [NOIP 2016 提高组] 玩具谜题 | 基础 / 模拟·环形 | 朝向取"当前所在小人"；一次平移代替逐格走；负数取模 `((x%n)+n)%n` | —— |
| [P2241](https://www.luogu.com.cn/problem/P2241) | 统计方形（数据加强版） | 基础 / 枚举·计数 | 按尺寸 $h\times w$ 分类，个数 $=(n-h+1)(m-w+1)$；$n$、$m$ 也要 `long long` | —— |
| [P2670](https://www.luogu.com.cn/problem/P2670) | [NOIP 2015 普及组] 扫雷游戏 | 基础 / 网格模拟 | 8 个方向写成偏移数组；雷格原样输出 `*`；行内无分隔符 | —— |
| [P3156](https://www.luogu.com.cn/problem/P3156) | 【深基15.例1】询问学号 | 基础 / 数组·随机访问 | 问"第几个"就是问下标，值域 $10^9$ 也不需要查找结构；$2\times10^6$ 的数组必须开全局 | [html](solutions/basic/p3156-student-id/visualization.html) |
| [P9750](https://www.luogu.com.cn/problem/P9750) | [CSP-J 2023] 一元二次方程 | 基础 / 数学·输出格式 | **骗分练习 ⑤**：特殊性质 ⇒ $\Delta$ 必为完全平方 ⇒ 只写有理分支稳过 6 个测试点；满分要根式化简 + 5 条格式逐字 | —— |
| [U397952](https://www.luogu.com.cn/problem/U397952) | [L1-006 AC数](solutions/basic/u397952-ac-count/README.md) | 基础 / 计数·前缀和 | 按右端点分类：遇 C 就加"左边 A 的个数"，O(n²) → O(n) | —— |
| [P1005](https://www.luogu.com.cn/problem/P1005) | [NOIP 2007 提高组] 矩阵取数游戏 | 动态规划 / 区间·高精度 | **骗分练习 ②**：60% 档承诺答案 $\le10^{16}$ ⇒ `long long` 白送 60 分；行与行完全独立 | —— |
| [P1499](https://www.luogu.com.cn/problem/P1499) | [CTSC2000] 公路巡逻 | 动态规划 / 时间轴·差分桶 | 两个整秒时刻定住一段直线 ⇒ 相遇判据三分支（$b=X$ 算、$a=T$ 不算），每辆车的限制是一个半区间，半区间能差分，转移从 $O(m)$ 降到 $O(1)$ | —— |
| [P14360](https://www.luogu.com.cn/problem/P14360) | [CSP-J 2025] 多边形 | 动态规划 / 背包计数 | 极值锚定 + 补集转化，m≥3 由判据自动蕴含 | [html](solutions/dp/p14360-polygon/visualization.html) |
| [P3017](https://www.luogu.com.cn/problem/P3017) | [USACO11MAR] Brownie Slicing G / 布朗尼切片 | 动态规划 / 二分答案·贪心 | 二分"最小块≥X"→带内贪心数块+带间DP选带；合格性对扩行单调使 DP 塌回贪心，O(R²C)→O(RC) | [html](solutions/dp/p3017-brownie/visualization.html) |
| [P7074](https://www.luogu.com.cn/problem/P7074) | [CSP-J 2020] 方格取数 | 动态规划 / 前缀最优 | **骗分练习 ④**：每列单向 ⇒ DFS 20 分 / $O(n^2m)$ 70 分 / $O(nm)$ 100 分三级阶梯，三版互拍 | —— |
| [P1160](https://www.luogu.com.cn/problem/P1160) | 队列安排 | 数据结构 / 双向链表 | 编号本身就是节点号 ⇒ 定位 $O(1)$，0 号哨兵把链接成环免特判；**重复删除必须用 `gone[]` 挡住（样例查不出来）** | —— |
| [P1241](https://www.luogu.com.cn/problem/P1241) | 括号序列 | 数据结构 / 栈 | 右括号类型不符时右括号作废，栈顶左括号**不弹出** | [html](solutions/ds/p1241-bracket-sequence/visualization.html) |
| [P1540](https://www.luogu.com.cn/problem/P1540) | [NOIP 2010 提高组] 机器翻译（队列版） | 数据结构 / 队列·FIFO缓存 | 与上面"基础 / 模拟"那行是**同一道题的两份实现**：这版只用 `queue` + `bool in[]`，命中时什么都不做（FIFO 不是 LRU） | —— |
| [P1996](https://www.luogu.com.cn/problem/P1996) | 约瑟夫问题 | 数据结构 / 队列·模拟 | 每轮只搬 $m-1$ 个人到队尾，报 $m$ 的那个留在队头出圈；$m=1$ 是最快的自检用例 | —— |
| [P2058](https://www.luogu.com.cn/problem/P2058) | [NOIP 2016 普及组] 海港 | 数据结构 / 队列·滑窗 | 窗口左开右闭 $(t_i-86400,\,t_i]$，过期判定必须写 `<=` | [html](solutions/ds/p2058-harbour/visualization.html) |
| [P2234](https://www.luogu.com.cn/problem/P2234) | [HNOI2002] 营业额统计 | 数据结构 / 有序集合·前驱后继 | "离我最近的历史数据"只可能在前驱或后继里；`it!=end()` 管后继、`it!=begin()` 管前驱，两个判断缺一不可 | —— |
| [P3613](https://www.luogu.com.cn/problem/P3613) | 【深基15.例2】寄包柜 | 数据结构 / 稀疏存储·键编码 | $a_i$ 未知就是提示：别开二维表，只存被点名的 $(i,j)$；乘数要**严格**大于格子号上界（取 100001） | —— |
| [P4387](https://www.luogu.com.cn/problem/P4387) | 【深基15.习9】验证栈序列 | 数据结构 / 栈·模拟 | 入栈序列是任意排列；一次压入后要连续弹出，判据是"全部弹出"而非"栈空" | [html](solutions/ds/p4387-validate-stack-sequences/visualization.html) |
| [P1095](https://www.luogu.com.cn/problem/P1095) | [NOIP 2005 提高组] 守望者的逃离 | 贪心 / 枚举·DP | **骗分练习 ③**：三条规则漏一条就掉一半分（反面教材实测只对该 21%~25%）；无魔法上限 ⇒ 枚举闪光次数 $k$ | —— |
| [P14357](https://www.luogu.com.cn/problem/P14357) | [CSP-J 2025] 拼数 | 贪心 / 计数排序 | 位数用满 + 降序交换论证 | [html](solutions/greedy/p14357-number/visualization.html) |
| [P14359](https://www.luogu.com.cn/problem/P14359) | [CSP-J 2025] 异或和 | 贪心 / 前缀异或 | 前缀异或配对 + 最多不相交区间最早结束贪心 | [html](solutions/greedy/p14359-xor/visualization.html) |
| [T228758](https://www.luogu.com.cn/problem/T228758) | [L1-008 字符串](solutions/greedy/t228758-string/README.md) | 贪心 / 字符串字典序 | 两个"非空"把首尾钉死 ⇒ 只看下一个字符与 s2[0]，严格更小才延长 | —— |

> 上表标 **骗分练习 ①~⑤** 的五题 + 五道纯模拟题，配套教案见 [notes/partial-score-handbook.md](notes/partial-score-handbook.md)（骗分 / 部分分技术手册，含每个技术点的实测证据与考场时间预算表）。


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
| ★ [notes/partial-score-handbook.md](notes/partial-score-handbook.md) | **骗分 / 部分分技术手册**：①把数据范围翻译成"允许的复杂度"（附本机实测换算尺）②10 个技术点，每个都指向库里一道题 + 一组实测数字 ③考场 3.5 小时时间预算表与红线 ④10 题实测汇总 ⑤已核实的备选题池 | 本批 10 题**不做 html 动画**，改用各 README 里的「板书演示」表格（可当堂投影 + 让学生手抄） |
| ★ [notes/cspj-trend-2021-2025.md](notes/cspj-trend-2021-2025.md) | **CSP-J 第二轮考情趋势分析（2021—2025）**：五年逐题表 + 算法考核清单（带优先级）+ T1—T4 位置指纹 + 16 课时排课建议。重点核对 2023/2024/2025 | —— |
| ★ [notes/mock-cspj-round2/](notes/mock-cspj-round2) | **自编 CSP-J 第二轮模拟卷 4 套 × 4 题（不是任何一年真题）**，按上面的趋势逐题仿出：`paper.html` 是可截图的仿真题面，每题另有题解 + 分步可视化，答案在 `answer-key.md` | 16 个（每题一个 `visualization.html`），题面排版另有 `sheetshots.js` 逐页截图核对 |
| ★ [notes/ccf-zhenti-2026-j1/](notes/ccf-zhenti-2026-j1) | **CCF 官方 2026 CSP-J 第一轮真题卷**（2026-09-19 考试，12 页 / 100 分），全 43 题零基础讲评：单选 15 + 阅读程序 3 篇 16—33 + 完善程序 2 篇 34—43。题面逐字录入在 `problem.txt`，好读版在 `paper.md`；原卷第 9 页缺失、39—43 题选项未印，已在文档里标出来没有补写 | BFS 入队顺序（题 8）、二进制位统计、高精度加法竖式、逐位扩展质数 dfs、进制减半"移位×m + 按 n 进位"、平衡分割递归树 + 平均值柱状图（共 6 个，见讲义 §0.1） |
| [notes/luogu-2026-j1/](notes/luogu-2026-j1) | **2026 第一轮 · 洛谷 SCP-J1 卷（洛谷命制，不是 CCF 真题）**，42 题零基础讲评，题面全文见 `paper.md` | 孪生素数、二维 dp 填表、九连环 dfs、二分第 k 小、分层 BFS 迷宫（共 5 个，见讲义 §0.1） |
| [notes/luogu-2026-s1/](notes/luogu-2026-s1) | **2026 第一轮 · 洛谷 SCP-S1 卷（洛谷命制，不是 CCF 真题）**，43 题零基础讲评，题面全文见 `paper.md` | permanent 状压 dp、Fibonacci 词 + Zeckendorf、异或哈希必经边（共 3 个，见讲义 §0.1） |
| [notes/mock-cspjs-round1/](notes/mock-cspjs-round1) | 自编仿真题集（**不是任何一份真题**，按官方题型结构 100 分出的练习） | [j-trace.html](notes/mock-cspjs-round1/j-trace.html)、[s-trace.html](notes/mock-cspjs-round1/s-trace.html) |

## 参考项目

本仓库的组织方式参考了以下开源项目：

- [OI-wiki](https://github.com/OI-wiki/OI-wiki) — 主题分类体系（`dp` / `ds` / `graph` / `string` …）
- [a1fredbao/OI-Solutions](https://github.com/a1fredbao/OI-Solutions) — 按平台分目录的题解记录格式
- [EndlessCheng/codeforces-go](https://github.com/EndlessCheng/codeforces-go) — 按专题归纳的题解与模板
- [cp-algorithms](https://github.com/cp-algorithms/cp-algorithms) — 算法原理讲解结构
