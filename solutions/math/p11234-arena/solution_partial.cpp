#include <bits/stdc++.h>
using namespace std;
// P11234 [CSP-S 2024] 擂台游戏 —— 骗分版（本目录不实现正解）
//
// 核心：把"可能夺冠的选手集合"算准。能力值只有和轮数比大小这一个用途，
//       所以把 [0, 2^31) 压成 K+1 个桶：bucket = min(a, K)。
// 路径 1：对每个"不同的询问 c"跑一次精确 DP（两路合并，逐轮淘汰）；
// 路径 2：所有 c_i 都是 2 的幂（特殊性质 A）时，没有补充选手，整场确定，直接 O(s) 模拟。
//
// 编译（必须 -static，本机两套 MinGW 冲突）：
//   g++ -static -O2 -std=c++14 solution_partial.cpp -o E:/ai/suanfa_study/.raw/bin/p11234_partial.exe

int n, m, K;
vector<int> ap;        // a'_i，1 起
vector<int> qs;        // 询问 c_i
vector<string> drow;   // drow[h] = 第 h 轮抽签串，长度 2^(K-h)，字符 '0'/'1' 连写

// 精确 DP：前缀 c 补人到 s = 2^k，返回"可能夺冠的选手编号和"
// 每层的所有可能胜者压平进一个 uint64 数组：高 32 位 = 桶集合 mask，低 32 位 = 选手编号
long long solveQuery(int c, const vector<unsigned>& bkt) {
    if (c == 1) return 1;                        // 只有 1 人：不用打比赛，1 号夺冠
    int k = 0; while ((1 << k) < c) ++k;
    int s = 1 << k;
    unsigned full = (1u << (K + 1)) - 1;         // 补充选手能力值任取 => 桶 [0..K] 全开
    static vector<unsigned long long> cur, nxt;
    static vector<int> off, noff;
    cur.resize(s); nxt.resize(s); off.resize(s + 1); noff.resize(s + 1);
    for (int p = 1; p <= s; ++p) {               // 叶子层：每个选手一个"0 层节点"
        unsigned mask = (p <= c) ? (1u << bkt[p]) : full;
        cur[p - 1] = ((unsigned long long)mask << 32) | (unsigned)p;
        off[p - 1] = p - 1;
    }
    off[s] = s;
    for (int h = 1; h <= k; ++h) {               // 两两合并，得到第 h 轮各场的可能胜者
        int cnt = s >> h;
        unsigned lowmask = (1u << h) - 1, kepmask = ~lowmask;  // 桶 < h：擂主"不够格"
        const char* row = drow[h].c_str();
        int tot = 0;
        noff[0] = 0;
        for (int j = 0; j < cnt; ++j) {          // 第一遍：统计新节点条目数
            int l0 = off[2 * j], l1 = off[2 * j + 1], l2 = off[2 * j + 2];
            int dd = row[j] - '0';               // 0 => 擂主是编号小的一侧(左)，1 => 右侧
            int a0 = dd ? l1 : l0, a1 = dd ? l2 : l1;           // 擂主一侧
            int b0 = dd ? l0 : l1, b1 = dd ? l1 : l2;           // 挑战者一侧
            int surv = 0; bool low = false;
            for (int u = a0; u < a1; ++u) {
                unsigned mask = (unsigned)(cur[u] >> 32);
                if (mask & kepmask) ++surv;      // 擂主存在"够格"的桶 => 他晋级
                if (mask & lowmask) low = true;  // 擂主也可能"不够格" => 对面整段晋级
            }
            noff[j + 1] = tot + surv + (low ? b1 - b0 : 0);
            tot = noff[j + 1];
        }
        int pos = 0;
        for (int j = 0; j < cnt; ++j) {          // 第二遍：填条目
            int l0 = off[2 * j], l1 = off[2 * j + 1], l2 = off[2 * j + 2];
            int dd = row[j] - '0';
            int a0 = dd ? l1 : l0, a1 = dd ? l2 : l1;
            int b0 = dd ? l0 : l1, b1 = dd ? l1 : l2;
            bool low = false;
            for (int u = a0; u < a1; ++u) {
                unsigned long long e = cur[u];
                unsigned mask = (unsigned)(e >> 32);
                if (mask & lowmask) low = true;
                unsigned keep = mask & kepmask;  // 晋级后擂主的可用桶被砍掉 "< h" 的部分
                if (keep) nxt[pos++] = ((unsigned long long)keep << 32) | (unsigned)(e & 0xffffffffu);
            }
            if (low)                             // 擂主不够格的情况：挑战者一侧全体带原 mask 晋级
                for (int u = b0; u < b1; ++u) nxt[pos++] = cur[u];
        }
        cur.swap(nxt); off.swap(noff);
    }
    long long A = 0;
    for (int u = off[0]; u < off[1]; ++u) A += (long long)(cur[u] & 0xffffffffu);
    return A;
}

// 特殊性质 A 快速通道：c = 2^k，没人被补充，整场擂台完全确定，冠军只有一个
long long simulateExact(int c, const vector<long long>& val) {
    int k = 0; while ((1 << k) < c) ++k;
    int s = 1 << k;
    vector<long long> w(s);                      // w[p] = 目前存活的选手编号
    for (int p = 0; p < s; ++p) w[p] = p + 1;
    for (int h = 1; h <= k; ++h) {               // 逐轮打，胜者前移
        int cnt = (int)w.size() >> 1;
        const string& row = drow[h];
        vector<long long> nw;
        for (int j = 0; j < cnt; ++j) {
            long long lo = w[2 * j], hi = w[2 * j + 1];          // lo < hi 恒成立
            int d = row[j] - '0';
            long long champ = d == 0 ? lo : hi;                  // 擂主
            long long other = d == 0 ? hi : lo;
            nw.push_back(val[champ] >= h ? champ : other);       // 擂主 a >= 轮数才赢
        }
        w.swap(nw);
    }
    return w[0];
}

int main() {
    scanf("%d%d", &n, &m);
    ap.assign(n + 1, 0);
    for (int i = 1; i <= n; ++i) scanf("%d", &ap[i]);
    qs.assign(m, 0);
    bool allPow2 = true;
    for (int i = 0; i < m; ++i) {
        scanf("%d", &qs[i]);
        int c = qs[i], k = 0;
        while ((1 << k) < c) ++k;
        if ((1 << k) != c) allPow2 = false;
    }
    K = 0; while ((1 << K) < n) ++K;
    drow.assign(K + 1, string());
    for (int h = 1; h <= K; ++h) { char buf[1 << 18]; scanf("%s", buf); drow[h] = buf; }
    int T; scanf("%d", &T);
    while (T--) {
        long long X[4];
        for (int j = 0; j < 4; ++j) scanf("%lld", &X[j]);
        vector<long long> val(n + 1);
        vector<unsigned> bkt(n + 1);
        for (int i = 1; i <= n; ++i) {           // 题面：a_i = a'_i ⊕ X_{i mod 4}，i 从 1 起
            val[i] = (long long)ap[i] ^ X[i % 4];
            bkt[i] = (unsigned)min(val[i], (long long)K);        // 桶：超过 K 一律记 K
        }
        vector<long long> memo(n + 1, -1);       // 每组数据的 memo 必须重新来（X 变了！）
        long long res = 0;
        for (int i = 1; i <= m; ++i) {
            int c = qs[i - 1];
            if (memo[c] < 0)
                memo[c] = allPow2 ? simulateExact(c, val) : solveQuery(c, bkt);
            res ^= memo[c] * (long long)i;       // (1*A_1) ⊕ (2*A_2) ⊕ ... ⊕ (m*A_m)
        }
        printf("%lld\n", res);
    }
    return 0;
}
