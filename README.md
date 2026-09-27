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

## 参考项目

本仓库的组织方式参考了以下开源项目：

- [OI-wiki](https://github.com/OI-wiki/OI-wiki) — 主题分类体系（`dp` / `ds` / `graph` / `string` …）
- [a1fredbao/OI-Solutions](https://github.com/a1fredbao/OI-Solutions) — 按平台分目录的题解记录格式
- [EndlessCheng/codeforces-go](https://github.com/EndlessCheng/codeforces-go) — 按专题归纳的题解与模板
- [cp-algorithms](https://github.com/cp-algorithms/cp-algorithms) — 算法原理讲解结构
