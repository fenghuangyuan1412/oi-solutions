#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

// ==== P1499 [CTSC2000] 公路巡逻 · 讲解代码（C++14）====
// 模型来源见 README「思路推导」。核心：DP + 差分桶把每段相遇数算成 O(1) 区间加。

const int BASE = 21600;                   // 6:00:00 换成秒 = 6*3600 = 21600
const int MAXN = 50;                       // 关口数上界（题面 1<n<50）
const int MAXT = BASE + 600 * MAXN + 605;  // 全局秒下标上界（最晚到达也不会超过）
const int INF  = 0x3f3f3f3f;               // 大数标记；两个 INF 相加仍不爆 int

struct Patrol { int T, X; };               // T=出发时刻, X=T+t=到达下一关口时刻
vector<Patrol> seg[MAXN];                  // 按“所在段”分桶：seg[s] = 第 s 段的巡逻车

// 滚动数组：f[cur][j] = 目标车在 j 秒到达“当前关口”的已遇最少巡逻车数
static int f[2][MAXT];

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    for (int i = 0; i < m; i++) {
        int ni, ti;
        char Ts[16];
        // T 是 HHMMSS（可能有前导 0），必须当字符串读；当整数读会丢前导 0 变十进制
        scanf("%d %s %d", &ni, Ts, &ti);
        int h  = (Ts[0] - '0') * 10 + (Ts[1] - '0');
        int mm = (Ts[2] - '0') * 10 + (Ts[3] - '0');
        int ss = (Ts[4] - '0') * 10 + (Ts[5] - '0');
        int T  = h * 3600 + mm * 60 + ss;   // HHMMSS → 当日秒数
        seg[ni].push_back({T, T + ti});     // ti 已保证 [300,600]
    }

    int cur = 0, nxt = 1;
    memset(f[0], 0x3f, sizeof f[0]);        // 0x3f 逐字节填充 → 每 int 恰为 INF
    memset(f[1], 0x3f, sizeof f[1]);
    f[cur][BASE] = 0;                        // 边界：第 1 关口 6:00 整出发，相遇 0

    int dif[305];                            // 差分桶：下标 k 对应 b = a+300+k (k=0..300)
    for (int i = 1; i <= n - 1; i++) {       // 逐段转移：关口 i → 关口 i+1
        int loA = BASE + 300 * (i - 1), hiA = BASE + 600 * (i - 1); // 关口 i 到达时刻窗口
        int loB = BASE + 300 * i,       hiB = BASE + 600 * i;       // 关口 i+1 到达时刻窗口
        for (int j = loB; j <= hiB; j++) f[nxt][j] = INF;           // 只清本窗口，避免整行 memset

        int cars = (int)seg[i].size();
        for (int a = loA; a <= hiA; a++) {
            int base = f[cur][a];
            if (base >= INF) continue;       // 该到达时刻不可达，跳过
            memset(dif, 0, sizeof(int) * 301);

            // 用半区间性质把每辆巡逻车对本段的“禁止 b”做成 O(1) 差分
            for (int c = 0; c < cars; c++) {
                int T = seg[i][c].T, X = seg[i][c].X;
                if (T == a) continue;                    // 同时出发 → 全程不算相遇
                int k = X - (a + 300);                   // 关键阈值：b 与 X 的分界落在 k
                if (T > a) {                             // 出发晚于目标车：命中 b>=X → 后缀
                    if (k <= 300) dif[k < 0 ? 0 : k]++;
                } else {                                 // T < a：命中 b<=X → 前缀
                    if (k >= 0) {
                        dif[0]++;
                        if (k + 1 <= 300) dif[k + 1]--;
                    }
                }
            }

            // 前缀和还原每个 b 的本段相遇数，并松弛 f[i+1][b]
            int acc = 0;
            for (int kk = 0; kk <= 300; kk++) {
                acc += dif[kk];
                int b = a + 300 + kk;
                int val = base + acc;
                if (val < f[nxt][b]) f[nxt][b] = val;
            }
        }
        int tmp = cur; cur = nxt; nxt = tmp;             // 滚动：关口 i+1 变成当前
    }

    // 第 n 关口的行现在是 f[cur]。先求全局最小，再取达到最小值的最小 j（最早到达）
    int lo = BASE + 300 * (n - 1), hi = BASE + 600 * (n - 1);
    int best = INF, bestJ = lo;
    for (int j = lo; j <= hi; j++) {
        if (f[cur][j] < best) { best = f[cur][j]; bestJ = j; } // 严格 <：保留最早的 j
    }
    printf("%d\n", best);
    int h = bestJ / 3600, mm = (bestJ % 3600) / 60, ss = bestJ % 60;
    printf("%02d%02d%02d\n", h, mm, ss);                 // 补前导零，6 位 HHMMSS
    return 0;
}
