# 使用说明

## 目录约定

```
solutions/<主题>/<平台><题号>-<题名 slug>/
├── README.md          # 题解正文（按 templates/solution-template.md 撰写）
├── solution.cpp       # 带逐段注释的 AC 代码
├── metadata.yml       # 机器可索引的元信息
└── visualization.html # 可选：交互式分步演示
```

主题目录与 OI-wiki 对齐，便于交叉查阅：

| 目录 | 主题 | 典型内容 |
| --- | --- | --- |
| `basic` | 基础 | 模拟、枚举、排序、前缀和、差分 |
| `dp` | 动态规划 | 线性、背包、区间、树形、状压、数位 |
| `ds` | 数据结构 | 并查集、堆、树状数组、线段树、平衡树 |
| `graph` | 图论 | 最短路、生成树、拓扑、二分图、网络流 |
| `search` | 搜索 | DFS、BFS、剪枝、双向、A* |
| `greedy` | 贪心 | 排序不等式、区间贪心、交换论证 |
| `math` | 数学 | 数论、组合、矩阵、高斯消元 |
| `string` | 字符串 | KMP、Trie、后缀、AC 自动机 |
| `geometry` | 计算几何 | 凸包、旋转卡壳、半平面交 |
| `_unsorted` | 暂存 | 新提交先落这里，再由 `scripts/index.py` 归位 |

## 命名规范

- 平台前缀：`P`（洛谷普及/算法）、`B`（洛谷原题）、`U`（洛谷校内）、`CF`（Codeforces）、`LB`（LibreOJ）、`S`（POJ）。
- 目录名：`P1048-heart-of-the-gold`，全小写，连字符分词。
- 一题一目录，**同一仓库内**，不按题建仓库。

## metadata.yml 字段

```yaml
id: P1048
title: 采药
source: luogu
url: https://www.luogu.com.cn/problem/P1048
topics: [dp/knapsack]
difficulty: 普及-
complexity:
  time: O(nm)
  space: O(m)
status: AC
visualization: true
```

`topics` 使用斜杠分层路径，索引脚本据此把题解归入对应主题目录并生成 README 表格。

## 可视化策略

1. **Markdown 内嵌 Mermaid**：GitHub 原生渲染，适合流程图、状态转移图，零依赖。
2. **独立 `visualization.html`**：单文件、内联 CSS/JS、无外部 CDN 依赖，适合数组状态、树结构、DFS/BFS 过程的分步动画。
3. 公式用 `$...$` / `$$...$$`，配合仓库根的 `mkdocs.yml`（Material 主题 + pymdownx.arithmatex）渲染。
