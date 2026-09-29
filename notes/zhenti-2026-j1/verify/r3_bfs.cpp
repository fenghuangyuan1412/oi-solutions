// r3_bfs.cpp —— 用完全独立的 BFS 复核第 3 篇程序的答案（不靠递归，所以不会爆栈）
//   状态 = (t 的位掩码, 上一次翻转的位置 lst)
//   边   = 选一个 i != lst 且 t[0..i-1] == s 的后缀(长度 i)，把第 i 位取反
//   答案 = 从 (0..., -1) 出发第一次到达 "t 全 1" 的步数 —— 与原题 dfs 的 min 完全等价
//   同时输出可达状态数 = 原 dfs 的调用次数
// 编译： g++ -static -O2 -std=c++14 -Wl,--stack,268435456 r3_bfs.cpp -o r3_bfs.exe
#include <bits/stdc++.h>
using namespace std;

int n;
unsigned int SMASK[25];           // s 的后缀 i 的位掩码（bit j 存 t[j] 应该是什么）
int lenOf[25];

// 返回在一个状态下所有合法的 i
static inline bool legal(int i, unsigned int T) {
    if (i == 0) return true;                       // 空串 == 空串，恒成立
    unsigned int need = SMASK[i];
    unsigned int mask = (1u << i) - 1;
    return (T & mask) == need;
}

long long bfsAnswer(const string& in, unsigned long long* reachable) {
    n = in.size();
    for (int i = 0; i <= n; i++) {
        unsigned int v = 0;
        for (int j = 0; j < i; j++)                 // t[j] 应等于 in[n-i+j]
            if (in[n - i + j] == '1') v |= (1u << j);
        SMASK[i] = v; lenOf[i] = i;
    }
    unsigned int goal = (1u << (n + 1)) - 1;
    int L = n + 2;                                  // lst 编码: lst+1 ∈ [0, n+1]
    size_t total = (size_t(1) << (n + 1)) * L;
    vector<unsigned long long> vis((total + 63) / 64, 0);
    vector<unsigned int> q;  q.reserve(1 << min(n + 1, 20));
    auto id = [&](unsigned int T, int l) { return (size_t)T * L + l; };
    auto setv = [&](size_t p) { vis[p >> 6] |= 1ull << (p & 63); };
    auto getv = [&](size_t p) { return (vis[p >> 6] >> (p & 63)) & 1; };

    q.push_back(0u); q.push_back(0u);                // 层哨兵：用 (size,len) 队列 + 距离变量
    vector<pair<unsigned int,int>> cur, nxt;
    cur.push_back({0u, 0});                          // lst=-1 -> 编码 0
    setv(id(0u, 0));
    long long cnt = 1, d = 0;
    while (!cur.empty()) {
        bool found = false;
        for (size_t idx = 0; idx < cur.size(); ++idx) {
            unsigned int T = cur[idx].first;
            int lc = cur[idx].second;
            int lst = lc - 1;
            if (T == goal) found = true;
            for (int i = 0; i <= n; i++) {
                if (i == lst) continue;
                if (!legal(i, T)) continue;
                unsigned int T2 = T ^ (1u << i);
                size_t p = id(T2, i + 1);
                if (getv(p)) continue;
                setv(p); cnt++;
                nxt.push_back({T2, i + 1});
            }
        }
        if (found) { if (reachable) *reachable = cnt; return d; }
        cur.swap(nxt); nxt.clear();
        d++;
    }
    if (reachable) *reachable = cnt;
    return -1;                                      // 不可达
}

int main() {
    const char* ins[] = {"00000001", "110111011110111110", "001000100001000001",
                         "10011101010", "01100010101", "1", "11", "01", "11111111",
                         "11111110", "00000000"};
    for (const char* p : ins) {
        string in = p;
        unsigned long long reach;
        long long a = bfsAnswer(in, &reach);
        printf("s=%-20s n=%2d  最少步数=%-8lld  可达状态数(=dfs调用次数)=%llu\n",
               in.c_str(), (int)in.size(), a, reach);
        fflush(stdout);
    }
    printf("\n--- 逐位取反后答案是否相同 (长度 1..12 穷举) ---\n");
    int diff = 0, tot = 0;
    for (int len = 1; len <= 12; len++) {
        for (int mask = 0; mask < (1 << len); mask++) {
            string s1;
            for (int i = 0; i < len; i++) s1 += char('0' + ((mask >> (len - 1 - i)) & 1));
            string s2 = s1;
            for (char& c : s2) c = (c == '0' ? '1' : '0');
            long long a1 = bfsAnswer(s1, 0), a2 = bfsAnswer(s2, 0);
            tot++;
            if (a1 != a2) { if (diff < 6) printf("  不同: %s->%lld  %s->%lld\n", s1.c_str(), a1, s2.c_str(), a2); diff++; }
        }
        printf("  len=%2d 完成 (不同 %d 例)\n", len, diff);
        fflush(stdout);
    }
    printf("  共 %d 对，取反后答案不同的 = %d\n", tot, diff);
    return 0;
}
