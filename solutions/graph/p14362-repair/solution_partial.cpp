#include <bits/stdc++.h>
using namespace std;
// P14362 [CSP-S 2025] 道路修复 —— 骗分版（本目录不实现 k=10 的正解）
//
// 精确的三条路：
//   k = 0                 ：一次 Kruskal（测试点 1~4）
//   所有 c_j=0 且都带 0 边 ：特殊性质 A，"全部乡镇都上"恰好一次 Kruskal
//   k <= 5 且非 A         ：枚举 2^k 个乡镇子集，每个子集跑一次 Kruskal
// 兜底（不保证正确）：k > 5 且非 A 时只试 {空集, 单个乡镇, 全集}，输出合法上界。
//
// 正确性关键：固定子集 S 时，答案 = sum c_j + MST(把 n+|S| 个点全部连通)。
//   对全局最优解实际用到的乡镇集合 J，MST(n+|J|) <= 最优解；反过来每个 S 的 MST 都是合法方案。
//   所以 min over S == 全局最优。枚举即精确。
//
// 编译（必须 -static，本机两套 MinGW 冲突）：
//   g++ -static -O2 -std=c++14 solution_partial.cpp -o E:/ai/suanfa_study/.raw/bin/p14362_partial.exe

struct Edge { int u, v, w; bool operator<(const Edge& o) const { return w < o.w; } };

// fread 快读：m = 10^6 时 scanf 要吃 1 秒以上，务必换掉
namespace FastIn {
    const int SZ = 1 << 20;
    char buf[SZ]; int idx = 0, len = 0;
    inline char gc() {
        if (idx == len) { len = (int)fread(buf, 1, SZ, stdin); idx = 0; if (!len) return 0; }
        return buf[idx++];
    }
    template<class T>
    inline bool readInt(T& out) {
        char c = gc(); if (!c) return false;
        while (c < '0' || c > '9') { c = gc(); if (!c) return false; }
        T num = 0;
        for (; c >= '0' && c <= '9'; c = gc()) num = num * 10 + (c - '0');
        out = num; return true;
    }
}

int n, m, k;
vector<Edge> base;              // 原有道路，已按 w 排序
vector<vector<Edge>> star;      // star[j]：乡镇 j 与城市的边（含乡镇端点），已排序
vector<long long> ct;           // 改造费 c_j

int fa[10020];
int findf(int x) { while (fa[x] != x) { fa[x] = fa[fa[x]]; x = fa[x]; } return x; }

// 固定乡镇子集：sum c_j + MST(全部 n + |towns| 个点连通)
long long calc(const vector<int>& towns) {
    int tot = n + (int)towns.size();
    vector<Edge> extra, buf;
    long long cost = 0;
    for (size_t t = 0; t < towns.size(); ++t) {
        int j = towns[t];
        cost += ct[j];
        extra.insert(extra.end(), star[j].begin(), star[j].end());
    }
    sort(extra.begin(), extra.end());
    buf.resize(base.size() + extra.size());
    merge(base.begin(), base.end(), extra.begin(), extra.end(), buf.begin());   // 两堆有序边线性合并
    for (int i = 1; i <= tot; ++i) fa[i] = i;                                   // 城市 + 本子集的乡镇
    int used = 0;
    for (size_t i = 0; i < buf.size() && used < tot - 1; ++i) {
        int x = findf(buf[i].u), y = findf(buf[i].v);
        if (x != y) { fa[x] = y; cost += buf[i].w; ++used; }
    }
    return cost;
}

int main() {
    FastIn::readInt(n); FastIn::readInt(m); FastIn::readInt(k);
    base.resize(m);
    for (int i = 0; i < m; ++i) { FastIn::readInt(base[i].u); FastIn::readInt(base[i].v); FastIn::readInt(base[i].w); }
    sort(base.begin(), base.end());
    star.assign(k, vector<Edge>());
    ct.assign(k, 0);
    bool allA = true;
    for (int j = 0; j < k; ++j) {
        FastIn::readInt(ct[j]);
        long long mn = LLONG_MAX;
        star[j].reserve(n);
        for (int i = 1; i <= n; ++i) {
            int a = 0; FastIn::readInt(a);
            mn = min(mn, (long long)a);
            star[j].push_back(Edge{n + 1 + j, i, a});    // 乡镇节点编号接在城市后面
        }
        sort(star[j].begin(), star[j].end());
        if (!(ct[j] == 0 && mn == 0)) allA = false;
    }

    long long best = LLONG_MAX;
    if (allA) {                                  // 特殊性质 A：全用上，恰好一次 Kruskal
        vector<int> towns;
        for (int j = 0; j < k; ++j) towns.push_back(j);
        best = calc(towns);
    } else if (k <= 5) {                         // 全枚举，精确
        for (int mask = 0; mask < (1 << k); ++mask) {
            vector<int> towns;
            for (int j = 0; j < k; ++j) if (mask >> j & 1) towns.push_back(j);
            best = min(best, calc(towns));
        }
    } else {                                     // 兜底：只试少数子集（合法上界，不一定最小）
        best = calc(vector<int>());              // 一个乡镇都不改造
        for (int j = 0; j < k; ++j) best = min(best, calc(vector<int>(1, j)));
        vector<int> towns;
        for (int j = 0; j < k; ++j) towns.push_back(j);
        best = min(best, calc(towns));
    }
    printf("%lld\n", best);
    return 0;
}
