# OI 题解知识库

信息学奥赛（CSP / NOIP / 洛谷 / Codeforces）题解仓库。每道题一个目录，包含题意压缩、思路推导、正确性论证、逐段讲解代码和可视化演示。

- 目录结构与写作规范见 [notes/CONVENTIONS.md](notes/CONVENTIONS.md)
- 新建题目：`./scripts/new_problem.sh P1048 dp/knapsack 采药`

## 题库索引

索引由题解目录自动汇总，新增题目后更新此处。

| 主题 | 题数 | 目录 |
| --- | --- | --- |
| 基础 | 14 | [solutions/basic](solutions/basic) |
| 动态规划 | 22 | [solutions/dp](solutions/dp) |
| 数据结构 | 8 | [solutions/ds](solutions/ds) |
| 图论 | 3 | [solutions/graph](solutions/graph) |
| 搜索 | 11 | [solutions/search](solutions/search) |
| 贪心 | 13 | [solutions/greedy](solutions/greedy) |
| 数学 | 2 | [solutions/math](solutions/math) |
| 字符串 | 0 | [solutions/string](solutions/string) |
| 计算几何 | 0 | [solutions/geometry](solutions/geometry) |

共 73 题（按"有 README.md 的目录"统计，本机 `find solutions -mindepth 3 -name README.md | wc -l` 实得 73；含 [solutions/search](solutions/search) 的 11 道搜索入门题，已在下表逐行列出）。全部题目来自训练单与洛谷/NOIP 真题，暂无半成品目录。

**怎么用这张表全局浏览**：

| 列 | 点下去到哪里 |
| --- | --- |
| 题号 | 洛谷原题页（题面、数据范围、评测） |
| 题目 | 本仓库该题的题解 `README.md`（题意 → 暴力 → 观察 → 算法 → 正确性 → 复杂度 → 讲解代码 → 易错点） |
| 可视化 | 该题的分步动画 `visualization.html`（单文件、内联 CSS/JS、离线可开，双击即可）；`——` 表示该题不做动画，改用题解里的「板书演示」表格 |
| 主题 / 目录 | 上表的「目录」列进入该主题的文件夹，能看到全部题目目录 |

每份题解顶部还有「本目录导航」一行，可在题目原文、讲解代码、元信息、对拍脚本之间直接跳转，并返回本页。
跨题的整套讲评与教案见下方「讲义索引（notes/）」一节，或直接进入 [`notes/`](notes) 目录。

### 题目列表

| 题号 | 题目 | 主题 | 核心思想 | 可视化 |
| --- | --- | --- | --- | --- |
| [P1003](https://www.luogu.com.cn/problem/P1003) | [[NOIP 2011 提高组] 铺地毯](solutions/basic/p1003-carpet/README.md) | 基础 / 模拟·枚举 | 后铺的在上 ⇒ 倒序枚举、第一个命中就 `break`；真涂满 $10^5\times10^5$ 要 40 GB | —— |
| [P1309](https://www.luogu.com.cn/problem/P1309) | [[NOIP 2011 普及组] 瑞士轮](solutions/basic/p1309-swiss-round/README.md) | 基础 / 排序·模拟 | **骗分练习 ①**：暴力每轮 `sort` 本身就值分；正解靠"胜者/负者两队列各自天然有序"一次归并 | —— |
| [P14358](https://www.luogu.com.cn/problem/P14358) | [[CSP-J 2025] 座位](solutions/basic/p14358-seat/README.md) | 基础 / 模拟·排序 | 名次 $k$ → 列 $\lceil k/n\rceil$，行按列号奇偶翻转 | [html](solutions/basic/p14358-seat/visualization.html) |
| [P1540](https://www.luogu.com.cn/problem/P1540) | [[NOIP 2010 提高组] 机器翻译](solutions/basic/p1540-machine-translation/README.md) | 基础 / 模拟·队列 | 淘汰规则是"最早进入"（FIFO）不是 LRU；命中时内存内容完全不动 | —— |
| [P1563](https://www.luogu.com.cn/problem/P1563) | [[NOIP 2016 提高组] 玩具谜题](solutions/basic/p1563-toypuzzle/README.md) | 基础 / 模拟·环形 | 朝向取"当前所在小人"；一次平移代替逐格走；负数取模 `((x%n)+n)%n` | —— |
| [P2241](https://www.luogu.com.cn/problem/P2241) | [统计方形（数据加强版）](solutions/basic/p2241-rect-count/README.md) | 基础 / 枚举·计数 | 按尺寸 $h\times w$ 分类，个数 $=(n-h+1)(m-w+1)$；$n$、$m$ 也要 `long long` | —— |
| [P2670](https://www.luogu.com.cn/problem/P2670) | [[NOIP 2015 普及组] 扫雷游戏](solutions/basic/p2670-minesweeper/README.md) | 基础 / 网格模拟 | 8 个方向写成偏移数组；雷格原样输出 `*`；行内无分隔符 | —— |
| [P3156](https://www.luogu.com.cn/problem/P3156) | [【深基15.例1】询问学号](solutions/basic/p3156-student-id/README.md) | 基础 / 数组·随机访问 | 问"第几个"就是问下标，值域 $10^9$ 也不需要查找结构；$2\times10^6$ 的数组必须开全局 | [html](solutions/basic/p3156-student-id/visualization.html) |
| [P9750](https://www.luogu.com.cn/problem/P9750) | [[CSP-J 2023] 一元二次方程](solutions/basic/p9750-quadratic/README.md) | 基础 / 数学·输出格式 | **骗分练习 ⑤**：特殊性质 ⇒ $\Delta$ 必为完全平方 ⇒ 只写有理分支稳过 6 个测试点；满分要根式化简 + 5 条格式逐字 | —— |
| [P9752](https://www.luogu.com.cn/problem/P9752) | [[CSP-S 2023] 密码锁](solutions/basic/p9752-lock/README.md) | 基础 / 枚举·规则翻译 | $10^5$ 个状态里只有 $10$ 个是"当前密码"，其余全靠规则反推 ⇒ **把题面那条"转一个圈 / 转相邻两个同幅度圈"逐字写成判定函数 `reachable()`，剩下交给全枚举**（$n\le8$，实测 $10\cdot 2^8$ 级别、极限档 6 ms）。骗分只需把判定函数"写窄"（只认单圈 + $n=1$ 特判）就能搬走档 1 与特殊性质 A；漏判"相邻同幅度"官方样例立刻从 81 变 369 | [html](solutions/basic/p9752-lock/visualization.html) |
| [P9754](https://www.luogu.com.cn/problem/P9754) | [[CSP-S 2023] 结构体](solutions/basic/p9754-struct/README.md) | 基础 / **大模拟**·内存对齐 | 不考算法、只考"把规格说明一字不差翻译成代码"：先对齐 `off`、再记偏移、最后 `off += size`，整体大小再补齐到对齐整数倍。**骗分价值极高**——四个特殊性质各砍一块实现量（A 无操作 4、B 只有一个操作 2、C 成员全基本类型免递归、D 只有 `long`） | —— |
| [P11227](https://www.luogu.com.cn/problem/P11227) | [[CSP-J 2024] 扑克牌](solutions/basic/p11227-cards/README.md) | 基础 / 计数·去重 | **重复不加分，缺种才要补**：答案 $=52-$ 手里出现过的不同牌种数，一张 `bool seen[4][13]` 点亮再数亮格就是全部正解（$O(n)$，$n\le52$）。押"两两不同"直接输出 `52-n` 只兜住特殊性质 A 的约 40 分——本机实测它在样例 2 上给 48（正解 49） | [html](solutions/basic/p11227-cards/visualization.html) |
| [P11228](https://www.luogu.com.cn/problem/P11228) | [[CSP-J 2024] 地图探险](solutions/basic/p11228-explore/README.md) | 基础 / 网格模拟 | 模拟本身没难度，**分差全在"怎么数不同的格子"**：`dx/dy` 方向表把"走/原地右转"压成两行，去重用 `bool vis[n][m]` 就是 $O(nm+k)$（最坏档实测 31 ms）；vector 线性查重的 $O(k^2)$ 暴力只够 $k\le2000$ 的前 60 分（同档实测 5319 ms 超时） | [html](solutions/basic/p11228-explore/visualization.html) |
| [U397952](https://www.luogu.com.cn/problem/U397952) | [L1-006 AC数](solutions/basic/u397952-ac-count/README.md) | 基础 / 计数·前缀和 | 按右端点分类：遇 C 就加"左边 A 的个数"，O(n²) → O(n) | —— |
| [P1005](https://www.luogu.com.cn/problem/P1005) | [[NOIP 2007 提高组] 矩阵取数游戏](solutions/dp/p1005-matrix-game/README.md) | 动态规划 / 区间·高精度 | **骗分练习 ②**：60% 档承诺答案 $\le10^{16}$ ⇒ `long long` 白送 60 分；行与行完全独立 | —— |
| [P1499](https://www.luogu.com.cn/problem/P1499) | [[CTSC2000] 公路巡逻](solutions/dp/p1499-patrol/README.md) | 动态规划 / 时间轴·差分桶 | 两个整秒时刻定住一段直线 ⇒ 相遇判据三分支（$b=X$ 算、$a=T$ 不算），每辆车的限制是一个半区间，半区间能差分，转移从 $O(m)$ 降到 $O(1)$ | —— |
| [P14360](https://www.luogu.com.cn/problem/P14360) | [[CSP-J 2025] 多边形](solutions/dp/p14360-polygon/README.md) | 动态规划 / 背包计数 | 极值锚定 + 补集转化，m≥3 由判据自动蕴含 | [html](solutions/dp/p14360-polygon/visualization.html) |
| [P9751](https://www.luogu.com.cn/problem/P9751) | [[CSP-J 2023] 旅游巴士](solutions/dp/p9751-bus/README.md) | 动态规划 / 分层图·BFS（**只讲骗分**） | "不许停留"让"越早到越好"失效，但"每 $k$ 一辆车"给了无穷多次重新出发 ⇒ 状态从"点"升维成"点 $+$ 步数 $\bmod k$"。两档骗分：$a_i=0$ 走分层 BFS，小数据走逐时刻模拟，正确性靠去环引理"最早离开 $\le \max a+(n+1)k$"。**正解（当年 J 组最难题之一）不实现**，为什么写不进本地验证在 README 里说清了 | —— |
| [P9753](https://www.luogu.com.cn/problem/P9753) | [[CSP-S 2023] 消消乐](solutions/dp/p9753-match/README.md) | 动态规划 / 栈·前缀状态 | 子串能消空 $\iff$ 它两端**前缀约简出来的栈一模一样** ⇒ "数子串"变成"数相同前缀状态的对数"；给栈内容树按 $(\text{父编号},\text{字符})$ 发号即 $O(26n)$（$2\times10^6$ 实测 23~29 ms）。**按"栈深"发号是错的**：官方样例输出 10（正确 5），最短反例 `aab` 输出 2（正确 1），已作为易错点 1 保留。阶梯四档：定义 DFS → $O(n^3)$ 区间 DP → $O(n^2)$ 逐左端点 → 性质 A/B 专档 | —— |
| [P11233](https://www.luogu.com.cn/problem/P11233) | [[CSP-S 2024] 染色](solutions/dp/p11233-color/README.md) | 动态规划 / 线性·**懒标记偏移** | 判分只看"最靠近的同色数"⇒ 每种颜色只需记"最后一个**值**"，自由度只剩一个；"同色转移 = 所有状态同时加同一个数"不真加，记 `shift` ⇒ $O(n)$（$T=10$ 极限实测 64 ms）。骗分版 $O(nV)$ 保留两档，按 20 点均分推算 65 分——$n\le2\times10^5$ 但 $A_i\le10$ 那档是"被 $n$ 吓住就会漏掉的"。忘加 `shift` 官方样例**照样输出 1 0 8** | —— |
| [P3017](https://www.luogu.com.cn/problem/P3017) | [[USACO11MAR] Brownie Slicing G / 布朗尼切片](solutions/dp/p3017-brownie/README.md) | 动态规划 / 二分答案·贪心 | 二分"最小块≥X"→带内贪心数块+带间DP选带；合格性对扩行单调使 DP 塌回贪心，O(R²C)→O(RC) | [html](solutions/dp/p3017-brownie/visualization.html) |
| [P7074](https://www.luogu.com.cn/problem/P7074) | [[CSP-J 2020] 方格取数](solutions/dp/p7074-grid/README.md) | 动态规划 / 前缀最优 | **骗分练习 ④**：每列单向 ⇒ DFS 20 分 / $O(n^2m)$ 70 分 / $O(nm)$ 100 分三级阶梯，三版互拍 | —— |
| [P1216](https://www.luogu.com.cn/problem/P1216) | [[IOI 1994 / USACO1.5] Number Triangles](solutions/dp/p1216-number-triangle/README.md) | 动态规划 / 入门·递推滚动 | 自底向上，整张表塌缩成一条滚动 f；塔底要原样抄进 f，写成 `f[j]=0` 本机实测样例从 30 变 25 | [html](solutions/dp/p1216-number-triangle/visualization.html) |
| [P1255](https://www.luogu.com.cn/problem/P1255) | [数楼梯](solutions/dp/p1255-staircase/README.md) | 动态规划 / 入门·递推·高精度 | 递推式就是斐波那契，门槛在数值：`int` 第 46 项就爆，$N=5000$ 答案 1045 位 ⇒ 手写竖式加法 | —— |
| [P1002](https://www.luogu.com.cn/problem/P1002) | [[NOIP 2002 普及组] 过河卒](solutions/dp/p1002-river-passage/README.md) | 动态规划 / 入门·网格计数 | 马的 9 个控制点直接置 0，$f[i][j]=上+左$；答案能到 8119857900，`int` 会回绕成 −470076692 | [html](solutions/dp/p1002-river-passage/visualization.html) |
| [P1044](https://www.luogu.com.cn/problem/P1044) | [[NOIP 2003 普及组] 栈](solutions/dp/p1044-stack-sequences/README.md) | 动态规划 / 入门·计数·卡特兰 | 最后一个进栈的元素把序列切成独立两半 ⇒ $f[n]=\sum f[k]f[n-1-k]$；$f[0]=1$ 是地基，漏了样例输出 0 | —— |
| [P1115](https://www.luogu.com.cn/problem/P1115) | [最大子段和](solutions/dp/p1115-max-subarray/README.md) | 动态规划 / 入门·线性 Kadane | `cur=max(x,cur+x)`；初值必须取第一个数而不是 0，否则全负数据输出 0（正解 −1） | —— |
| [P1091](https://www.luogu.com.cn/problem/P1091) | [[NOIP 2004 提高组] 合唱队形](solutions/dp/p1091-chorus-line/README.md) | 动态规划 / 入门·双向 LIS | 正反各跑一次 LIS，答案 $n-\max(up+down-1)$；峰被左右共用，忘减 1 时样例从 4 变 3 | —— |
| [P1048](https://www.luogu.com.cn/problem/P1048) | [[NOIP 2005 普及组] 采药](solutions/dp/p1048-herbs/README.md) | 动态规划 / 入门·01 背包 | 一维 f + 内层**倒序**是 01 背包的全部命门；写成正序就变成完全背包，本机实测样例从 3 变 140 | [html](solutions/dp/p1048-herbs/visualization.html) |
| [P1049](https://www.luogu.com.cn/problem/P1049) | [[NOIP 2001 普及组] 装箱问题](solutions/dp/p1049-box-packing/README.md) | 动态规划 / 入门·背包·体积当价值 | `f[j]=max(f[j],f[j-v]+v)`，问的是剩余空间 ⇒ 必须输出 $V-f[V]$，直接印 `f[V]` 样例从 0 变 24 | —— |
| [P1164](https://www.luogu.com.cn/problem/P1164) | [小 A 点菜](solutions/dp/p1164-order-dishes/README.md) | 动态规划 / 入门·背包计数 | "恰好花光"让 $\max$ 变成 $+$：$f[j]+=f[j-a]$ 且 $f[0]=1$；官方数据弱，但自造数据能把 `long long` 也打爆 | —— |
| [P1435](https://www.luogu.com.cn/problem/P1435) | [[IOI 2000] 回文字串](solutions/dp/p1435-palindrome-string/README.md) | 动态规划 / 入门·区间 DP | 两端相等白捡 $f[i+1][j-1]$，不等才 $\min+1$；填表必须先短后长，$1005^2$ 数组必须开全局 | —— |
| [P1616](https://www.luogu.com.cn/problem/P1616) | [疯狂的采药](solutions/dp/p1616-crazy-herbs/README.md) | 动态规划 / 入门·**完全背包** | 与 P1048 **样例逐字相同**、答案从 3 变 140：内层由**倒序改成正序**，$dp[j-a]$ 读到本轮新值 ⇒ 同一株可无限采；倒序版在 2000 组对拍里 1745 组偏小 | —— |
| [P1880](https://www.luogu.com.cn/problem/P1880) | [[NOI1995] 石子合并](solutions/dp/p1880-stone-merge/README.md) | 动态规划 / **环形区间 DP** | 最后一次合并得分恒为 $\text{sum}(i,j)$（与分割点无关）⇒ 转移干净；环形用**断环成链**（复制一倍 + 枚举剪开位置），忘了这一步 2000 组里 1098 组出错 | —— |
| [P1352](https://www.luogu.com.cn/problem/P1352) | [没有上司的舞会](solutions/dp/p1352-prom-without-boss/README.md) | 动态规划 / **树形 DP** | 每个节点两状态：$f[u][1]=r_u+\sum f[v][0]$、$f[u][0]=\sum\max(f[v][0],f[v][1])$，后序遍历；$f[u][0]$ 漏写 $\max$ 在 500 组里 415 组出错 | —— |
| [P3622](https://www.luogu.com.cn/problem/P3622) | [[APIO2007] 动物园](solutions/dp/p3622-zoo/README.md) | 动态规划 / 状压·轮廓线 | 窗口只滑一格 ⇒ 历史只剩 4 位，把"接下来 5 个围栏移不移"压成 5 位 mask；圈的问题枚举开头模式、走完一圈绕回原模式（初值必须是 0，样例 2 抓的重复结算坑） | [html](solutions/dp/p3622-zoo/visualization.html) |
| [P11855](https://www.luogu.com.cn/problem/P11855) | [\[CSP-J 2022 山东\] 部署](solutions/graph/p11855-deploy/README.md) | 图论 / 树·离线差分 | "子树"与"点到根的路径"对偶：$m$ 次子树加/邻域加，离线后一次前缀和 + 一次子→父累加全消化；$n,m,q$ 到 $10^6$ 须迭代 BFS + 链式前向星 | —— |
| [P2661](https://www.luogu.com.cn/problem/P2661) | [\[NOIP 2015 提高组\] 信息传递](solutions/graph/p2661-message-passing/README.md) | 图论 / 函数图·最小环 | 每人只告诉一个人 ⇒ 出度全 1 的函数图，每连通块恰有一环；游戏轮数 = 最短环长，三态标记 $O(n)$ 求环（不区分在链/已完结会误把长尾当环） | —— |
| [P14362](https://www.luogu.com.cn/problem/P14362) | [[CSP-S 2025] 道路修复](solutions/graph/p14362-repair/README.md) | 图论 / MST·子集枚举（**只讲骗分**） | 固定乡镇子集 $S$ 后答案 $=\sum c_j + \text{MST}$，再对 $S$ 取 min ⇒ "选哪几个乡镇"只有 $2^k$ 种，$k\le5$ 时每种跑一遍 Kruskal 就是精确解，72 分全是这么搬回来的；$k=10$ 卡在 $1024\times10^6$ 条边（实测 7689 ms / 13821 ms）正解不实现。考场命门是四件小事：`fread` 快读（scanf 读 20 MB 实测约 1.1 s）、别忘 $c_j$、`long long`、DSU 范围 | —— |
| [P1160](https://www.luogu.com.cn/problem/P1160) | [队列安排](solutions/ds/p1160-queue-arrangement/README.md) | 数据结构 / 双向链表 | 编号本身就是节点号 ⇒ 定位 $O(1)$，0 号哨兵把链接成环免特判；**重复删除必须用 `gone[]` 挡住（样例查不出来）** | —— |
| [P1241](https://www.luogu.com.cn/problem/P1241) | [括号序列](solutions/ds/p1241-bracket-sequence/README.md) | 数据结构 / 栈 | 右括号类型不符时右括号作废，栈顶左括号**不弹出** | [html](solutions/ds/p1241-bracket-sequence/visualization.html) |
| [P1540](https://www.luogu.com.cn/problem/P1540) | [[NOIP 2010 提高组] 机器翻译（队列版）](solutions/ds/p1540-machine-translation/README.md) | 数据结构 / 队列·FIFO缓存 | 与上面"基础 / 模拟"那行是**同一道题的两份实现**：这版只用 `queue` + `bool in[]`，命中时什么都不做（FIFO 不是 LRU） | —— |
| [P1996](https://www.luogu.com.cn/problem/P1996) | [约瑟夫问题](solutions/ds/p1996-josephus/README.md) | 数据结构 / 队列·模拟 | 每轮只搬 $m-1$ 个人到队尾，报 $m$ 的那个留在队头出圈；$m=1$ 是最快的自检用例 | —— |
| [P2058](https://www.luogu.com.cn/problem/P2058) | [[NOIP 2016 普及组] 海港](solutions/ds/p2058-harbour/README.md) | 数据结构 / 队列·滑窗 | 窗口左开右闭 $(t_i-86400,\,t_i]$，过期判定必须写 `<=` | —— |
| [P2234](https://www.luogu.com.cn/problem/P2234) | [[HNOI2002] 营业额统计](solutions/ds/p2234-turnover/README.md) | 数据结构 / 有序集合·前驱后继 | "离我最近的历史数据"只可能在前驱或后继里；`it!=end()` 管后继、`it!=begin()` 管前驱，两个判断缺一不可 | —— |
| [P3613](https://www.luogu.com.cn/problem/P3613) | [【深基15.例2】寄包柜](solutions/ds/p3613-locker/README.md) | 数据结构 / 稀疏存储·键编码 | $a_i$ 未知就是提示：别开二维表，只存被点名的 $(i,j)$；乘数要**严格**大于格子号上界（取 100001） | —— |
| [P4387](https://www.luogu.com.cn/problem/P4387) | [【深基15.习9】验证栈序列](solutions/ds/p4387-validate-stack-sequences/README.md) | 数据结构 / 栈·模拟 | 入栈序列是任意排列；一次压入后要连续弹出，判据是"全部弹出"而非"栈空" | [html](solutions/ds/p4387-validate-stack-sequences/visualization.html) |
| [P1752](https://www.luogu.com.cn/problem/P1752) | [点菜](solutions/greedy/p1752-ordering/README.md) | 贪心 / 二分答案·堆 | 改编自 IOI 2013 D2T2。时间轴塌缩成容量：最挑剔的先吃"够格里最贵的"（价格大根堆），穷人从最富的吃吃得起的最贵的，吃不起的甩给普通人兜底；二分上界是 $m$ 不是 $\lceil m/n\rceil$ | [html](solutions/greedy/p1752-ordering/visualization.html) |
| [P1182](https://www.luogu.com.cn/problem/P1182) | [数列分段 Section II](solutions/greedy/p1182-segment/README.md) | 贪心 / **二分答案·最小化最大值** | 判定"每段和 $\le X$ 能否用 $\le M$ 段"⇒ 贪心能塞就塞、$cnt\le M$ 即可行（段数不够可再拆细）；收口写成 `>=` 在 3000 组里 1128 组答案偏大 +1 | [html](solutions/greedy/p1182-segment/visualization.html) |
| [P2678](https://www.luogu.com.cn/problem/P2678) | [[NOIP 2015 提高组] 跳石头](solutions/greedy/p2678-jump-stone/README.md) | 贪心 / **二分答案·最大化最小值** | 判定"所有间距 $\ge X$"⇒ 太近就移走这块；终点前那一段要单独补一刀（移走刚保留的那块，不是终点），本题唯一的证明难点；附两组专打"多趟扫描重复计数"的 hack | [html](solutions/greedy/p2678-jump-stone/visualization.html) |
| [P1095](https://www.luogu.com.cn/problem/P1095) | [[NOIP 2005 提高组] 守望者的逃离](solutions/greedy/p1095-escape/README.md) | 贪心 / 枚举·DP | **骗分练习 ③**：三条规则漏一条就掉一半分（反面教材实测只对该 21%~25%）；无魔法上限 ⇒ 枚举闪光次数 $k$ | —— |
| [P14357](https://www.luogu.com.cn/problem/P14357) | [[CSP-J 2025] 拼数](solutions/greedy/p14357-number/README.md) | 贪心 / 计数排序 | 位数用满 + 降序交换论证 | [html](solutions/greedy/p14357-number/visualization.html) |
| [P14359](https://www.luogu.com.cn/problem/P14359) | [[CSP-J 2025] 异或和](solutions/greedy/p14359-xor/README.md) | 贪心 / 前缀异或 | 前缀异或配对 + 最多不相交区间最早结束贪心 | [html](solutions/greedy/p14359-xor/visualization.html) |
| [P9749](https://www.luogu.com.cn/problem/P9749) | [[CSP-J 2023] 公路](solutions/greedy/p9749-road/README.md) | 贪心 / **历史最低价** | 油箱无限大 ⇒ 可以在最便宜的那个站**提前买油**；维护 `minp`（历史最低价）+ `rest`（还能跑多远）一趟扫完。答案可达 $10^{15}$ 必须 `long long`；"整数升"必须向上取整 | [html](solutions/greedy/p9749-road/visualization.html) |
| [P9755](https://www.luogu.com.cn/problem/P9755) | [[CSP-S 2023] 种树](solutions/greedy/p9755-tree/README.md) | 贪心 / **二分答案**·树上 | **只讲骗分、不实现正解**：答案单调 ⇒ 二分是唯一救命稻草；链上种树顺序唯一可贪心，菊花上贪心失效而二分有效。数值坑：累计高度可达 $10^{23}$ 必须 `__int128` | —— |
| [P11231](https://www.luogu.com.cn/problem/P11231) | [[CSP-S 2024] 决斗](solutions/greedy/p11231-duel/README.md) | 贪心 / 二分图最大匹配 | "每只至多攻击一次"+"被杀就退出"正是匹配的两条边约束 ⇒ 答案 $=n-$ 最大匹配；排序后双指针四行搞定。读题陷阱：**已攻击过的怪兽仍可被杀**（样例 1 第 3 回合） | [html](solutions/greedy/p11231-duel/visualization.html) |
| [P11230](https://www.luogu.com.cn/problem/P11230) | [[CSP-J 2024] 接龙](solutions/greedy/p11230-dragon/README.md) | 贪心 / 可达性·记忆化（**只讲骗分**） | "能否恰好 $r$ 轮接到 $c$"本质是**带限制的可达性** ⇒ 骗分次序跟着结构走：先拿约束全消的 $r=1$ 档（查一轮可达表即 $O(1)$，$n=10^5$ 实测 0.28 s），再拿小 $n$ 小 $r$ 的记忆化 DFS，满档（$n,r,q$ 同时 $10^5$）要分层 BFS，正解不实现。漏"相邻两轮不同人"官方样例就从 `1 0 1 0 1 0 0` 变 `1 0 1 1 1 0 0` | —— |
| [P11232](https://www.luogu.com.cn/problem/P11232) | [[CSP-S 2024] 超速检测](solutions/greedy/p11232-speed/README.md) | 贪心 / **区间打点**·整数判据 | 速度平方 $g(x)=v^2+2a(x-d)$ 对位置单调 ⇒ "超速"是一段连续区间，且判据全程整数 $g>V^2$ **直接绕开题面警告的浮点精度**；第二问 = 区间打点贪心（按右端点排序、遇未覆盖就钉右端点）。$n=m=10^5$ 实测 0.30 s，$O(nm)$ 暴力同档 7.86 s。把"到达限速"也算超速（`>` 写成 `>=`）样例第一问立刻从 3 变 4 | —— |
| [P14361](https://www.luogu.com.cn/problem/P14361) | [[CSP-S 2025] 社团招新](solutions/greedy/p14361-club/README.md) | 贪心 / 交换论证 | **先按最喜欢的选（不管限制），再只修那一处被破坏的限制**：超载部门至多一个、接收部门空位一定够 ⇒ "请谁出去"退化成"取最小的 $r$ 个改派代价"，$O(n\log n)$。想不通这步就先写 $O(n\cdot(n/2)^2)$ 计数 DP 搬 $n\le200$ 的 55 分。最阴的坑：`c3=(i-1)-c1-c2` 写成 `i-c1-c2`，官方样例照样输出 `18 4 13` | [html](solutions/greedy/p14361-club/visualization.html) |
| [T228758](https://www.luogu.com.cn/problem/T228758) | [L1-008 字符串](solutions/greedy/t228758-string/README.md) | 贪心 / 字符串字典序 | 两个"非空"把首尾钉死 ⇒ 只看下一个字符与 s2[0]，严格更小才延长 | —— |
| [P1036](https://www.luogu.com.cn/problem/P1036) | [[NOIP 2002 普及组] 选数](solutions/search/p1036-choose-prime/README.md) | 搜索 / DFS·组合枚举 | 枚举归搜索（下一层起点 = 上一个下标 + 1，`sum` 当参数带走所以回溯免费），判定归数学（$v<2$ 先否、只试除到 $i\times i\le v$） | —— |
| [P1135](https://www.luogu.com.cn/problem/P1135) | [奇怪的电梯](solutions/search/p1135-strange-elevator/README.md) | 搜索 / BFS·最短路 | "状态只有 $n$ 个、每状态固定两个动作"就是 BFS 的信号：楼层当点、按键当边，`dist` 的 $-1$ 初值兼职 vis 与"无解"输出 | —— |
| [P1157](https://www.luogu.com.cn/problem/P1157) | [组合的输出](solutions/search/p1157-combination/README.md) | 搜索 / DFS·组合 | 组合的唯一直觉：下一层从"上一个数 + 1"开始 ⇒ 自动不重复、行内升序、行间字典序，不需要 `vis[]` | —— |
| [P1162](https://www.luogu.com.cn/problem/P1162) | [填涂颜色](solutions/search/p1162-fill-color/README.md) | 搜索 / DFS·反向洪泛 | "到不了边界"逆过来做：从**整条边界**洪泛标圈外，剩下的就是圈内，$O(n^4)\to O(n^2)$；**只从一个角出发在样例上碰巧对** | [html](solutions/search/p1162-fill-color/visualization.html) |
| [P1219](https://www.luogu.com.cn/problem/P1219) | [[USACO1.5] 八皇后 Checker Challenge](solutions/search/p1219-eight-queens/README.md) | 搜索 / 回溯·剪枝 | 按行放子让"每行每列恰一个"变成免费约束，两条对角线用 $i+j$、$i-j$ 各一张表 $O(1)$ 查；标记→递归→**撤销** | [html](solutions/search/p1219-eight-queens/visualization.html) |
| [P1443](https://www.luogu.com.cn/problem/P1443) | [马的遍历](solutions/search/p1443-knight-move/README.md) | 搜索 / BFS·网格 | 按层扩散 ⇒ 第一次到达即最少步；入队即打标记，`dist` 的 $-1$ 既是 vis 又是"到不了"的答案 | [html](solutions/search/p1443-knight-move/visualization.html) |
| [P1451](https://www.luogu.com.cn/problem/P1451) | [求细胞数量](solutions/search/p1451-cells/README.md) | 搜索 / DFS·连通块计数 | 数块模板：扫到未标记的目标格就 `ans++` 并洪泛整块、标记永不撤销；陷阱全在"四方向 + 非 `0` 即同类" | —— |
| [P1596](https://www.luogu.com.cn/problem/P1596) | [[USACO10OCT] Lake Counting S / 数水塘](solutions/search/p1596-lake-counting/README.md) | 搜索 / DFS·连通块计数 | 与 P1451 同一套模板，**唯一代码差异**是八方向偏移表：斜角相连也算同一个塘 | —— |
| [P1605](https://www.luogu.com.cn/problem/P1605) | [迷宫](solutions/search/p1605-maze/README.md) | 搜索 / DFS·回溯计数 | 数"块"不撤销、数"路"必须撤销 `vis` —— 这一行写不写就是两类搜索题的分界线 | —— |
| [P1706](https://www.luogu.com.cn/problem/P1706) | [全排列问题](solutions/search/p1706-permutation/README.md) | 搜索 / DFS·回溯 | 回溯三句话：做了什么 → 往下递归 → 撤销什么；每层从 $1..n$ 试、靠 `vis[]` 去重，`setw(5)` 场宽是格式分 | —— |
| [P11229](https://www.luogu.com.cn/problem/P11229) | [[CSP-J 2024] 小木棍](solutions/search/p11229-sticks/README.md) | 搜索 / DFS 回溯 → 贪心构造 | **"位数少先摆小数字"的贪心，要先有充要可行性判据**：$k$ 个空位装 $r$ 根 $\iff 2k\le r\le 7k$ ⇒ 答案位数 $\lceil n/7\rceil$ 加上逐位贪心，$O(n)$（50 组 $n=10^5$ 实测 74 ms）。DFS 骗分每多悟一条剪枝就离正解近一步：补上界剪枝后 $n=18$ 的结点从 123 掉到 13，回溯自动消失；裸 DFS 只到 $n\le50$（实测 $n=70$ 20 秒跑不完）。性质 A/B 各是一行结论 | [html](solutions/search/p11229-sticks/visualization.html) |
| [P9748](https://www.luogu.com.cn/problem/P9748) | [[CSP-J 2023] 小苹果](solutions/math/p9748-apple/README.md) | 数学 / 递推·模拟 | 只维护"还剩几个 `cnt`"和"目标排第几 `pos`"两个量 ⇒ 把 $O(n)$ 模拟压成 $O(\log n)$ 递推；$n\le10^9$ 直接堵死数组方案。特殊性质"第一天就取走 $n$"可把第二问写死为 1 | [html](solutions/math/p9748-apple/visualization.html) |
| [P11234](https://www.luogu.com.cn/problem/P11234) | [[CSP-S 2024] 擂台游戏](solutions/math/p11234-arena/README.md) | 数学 / 分桶·逐询问 DP（**只讲骗分**） | difficulty=8 的压轴也能搬走 44 分，靠的是把 $2^{31}$ 种能力值压成 $K+1$ 个桶（$a$ 只跟轮数 $R\le K\le17$ 比大小），"补充选手任取"从无穷种变成可枚举；再逐轮合并"可能胜者 $(\text{选手},\text{桶集合})$"对每个不同询问精确 DP。**正解不实现**。最阴的易错点实测只错一组：补充选手桶少一位 ⇒ `18/19/7/1` | —— |

> 上表标 **骗分练习 ①~⑤** 的五题 + 五道纯模拟题，配套教案见 [notes/partial-score-handbook.md](notes/partial-score-handbook.md)（骗分 / 部分分技术手册，含每个技术点的实测证据与考场时间预算表）。
>
> 上表标 **（只讲骗分）** 的五题（P9751 / P9755 / P11230 / P11234 / P14362）与其余 CSP-J/S 2023—2025 真题同批写就，按洛谷 difficulty 分档：**difficulty ≤ 4 的题同时给正解与骗分阶梯**（P9751 是唯一例外——正解是当年 J 组最难题之一，只搬分层 BFS 那两档）；**difficulty ≥ 5 的题默认只给按档搬分的骗分版并写清正解为什么不落地**，只有 P9753 / P11233 两步观察能板书的连正解一起实现。考情趋势见 [notes/cspj-trend-2021-2025.md](notes/cspj-trend-2021-2025.md)。


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
| ★ [notes/dp-binary-answer-selection.md](notes/dp-binary-answer-selection.md) | **DP 与二分答案 · 典型题选题讲义（分层递进）**：DP 与二分答案各 24 道典型题，按 L1 入门 / L2 提高 / L3 拔高三层排布，每题给出「教学切入点」一句话；含与库内已有题的对照表、两套各 6 课时的排课表、后续批次生成优先级 | 本批 5 道新题里 P1182 / P2678 配分步动画，其余 3 道用题解内的「板书演示」表格 |
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
