// 2026 CSP-J1 一、单项选择题（1—15）机器实测
// 编译：g++ -static -O2 -std=c++14 mc_choice.cpp -o mc_choice.exe
// 只做卷面问的那件事，不做任何"我觉得"：每题独立算一遍并打印，方便和卷面选项逐字对。
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <vector>
using namespace std;

// ---------- 题 1：谁能精确存下 10^18 + 1 ----------
static void q1() {
    const long long v = 1000000000000000000LL + 1;  // 10^18 + 1
    printf("题1  sizeof(float)=%zu sizeof(double)=%zu sizeof(long long)=%zu\n",
           sizeof(float), sizeof(double), sizeof(long long));
    printf("     long long  存 %lld -> 读回 %lld  %s\n", v, v, (v == 1000000000000000001LL) ? "精确" : "不精确");
    float f = (float)v;
    double d = (double)v;
    printf("     float  读回 %.0f  差 %+lld\n", f, (long long)f - v);
    printf("     double 读回 %.0f  差 %+lld\n", d, (long long)d - v);
    printf("     float 有效位 %d, double 有效位 %d（十进制）\n",
           numeric_limits<float>::digits10, numeric_limits<double>::digits10);
}

// ---------- 题 2：(2F5)16 -> 八进制 ----------
static void q2() {
    int x = 0x2F5;
    printf("题2  (2F5)16 = 十进制 %d = 八进制 %o\n", x, x);
}

// ---------- 题 3：a/b*b + a%b ----------
static void q3() {
    int a = 7, b = 3;
    printf("题3  a/b*b + a%%b = %d\n", a / b * b + a % b);
}

// ---------- 题 4：1,2,3,4 依次入栈，哪个出栈序列不可能 ----------
static bool achievable(const vector<int>& target) {
    // 枚举所有合法的入/出栈操作序列，看能否得到 target
    vector<int> st, out;
    bool ok = false;
    function<void(int)> dfs = [&](int nxt) {
        if (ok) return;
        if (out.size() == 4) { ok = (out == target); return; }
        if (nxt <= 4) {  // 入栈
            st.push_back(nxt); out.empty();
            dfs(nxt + 1);
            st.pop_back();
        }
        if (!st.empty()) {  // 出栈
            int t = st.back(); st.pop_back(); out.push_back(t);
            dfs(nxt);
            out.pop_back(); st.push_back(t);
        }
    };
    dfs(1);
    return ok;
}
static void q4() {
    vector<vector<int>> cand = {{2, 4, 3, 1}, {1, 2, 3, 4}, {3, 1, 2, 4}, {1, 4, 3, 2}};
    const char* nm = "ABCD";
    for (int i = 0; i < 4; i++) {
        string s;
        for (int v : cand[i]) s += to_string(v) + ",";
        s.pop_back();
        printf("题4  %c. {%s} -> %s\n", nm[i], s.c_str(), achievable(cand[i]) ? "可能" : "不可能");
    }
}

// ---------- 题 5：100 个结点的完全二叉树，叶子个数 ----------
static void q5() {
    int leaf = 0;
    for (int i = 1; i <= 100; i++)
        if (2 * i > 100) leaf++;  // 无左孩子 => 叶子
    printf("题5  100 个结点的完全二叉树：叶子 %d 个（编号 1..100 中 2i>100 的有 %d 个）\n", leaf, leaf);
}

// ---------- 题 6：1..100 中 3 或 5 的倍数之和 ----------
static void q6() {
    int s = 0;
    for (int i = 1; i <= 100; i++)
        if (i % 3 == 0 || i % 5 == 0) s += i;
    printf("题6  按卷面代码跑出来 s = %d\n", s);
}

// ---------- 题 7：每步 1/2/3 级，走到第 8 级的走法数 ----------
static void q7() {
    long long f[9] = {1, 0, 0, 0, 0, 0, 0, 0, 0};
    f[0] = 1;
    for (int i = 1; i <= 8; i++) {
        f[i] = 0;
        for (int k = 1; k <= 3; k++)
            if (i - k >= 0) f[i] += f[i - k];
    }
    printf("题7  f(0..8) =");
    long long g[9] = {1, 1, 2, 4, 7, 13, 24, 44, 81};
    for (int i = 0; i <= 8; i++) printf(" %lld", f[i]);
    printf("\n     走到第 8 级共 %lld 种\n", f[8]);
    (void)g;
}

// ---------- 题 8：5x5 网格 BFS，E 第一次入队时已入队过的格子数 ----------
static void q8() {
    const char* g[5] = {"S..#.", "...#.", "...#.", "##..E", "...#."};
    int sr = -1, sc = -1, er = -1, ec = -1;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++) {
            if (g[i][j] == 'S') { sr = i; sc = j; }
            if (g[i][j] == 'E') { er = i; ec = j; }
        }
    bool vis[5][5];
    memset(vis, 0, sizeof vis);
    // 上、下、左、右
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    queue<pair<int, int>> que;
    que.push({sr, sc});
    vis[sr][sc] = true;
    int enq = 1;
    vector<string> order;
    order.push_back("(" + to_string(sr) + "," + to_string(sc) + ")");
    while (!que.empty()) {
        int r = que.front().first, c = que.front().second;
        que.pop();
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr > 4 || nc < 0 || nc > 4) continue;
            if (g[nr][nc] == '#') continue;
            if (vis[nr][nc]) continue;
            vis[nr][nc] = true;
            que.push({nr, nc});
            enq++;
            order.push_back("(" + to_string(nr) + "," + to_string(nc) + ")");
            if (nr == er && nc == ec) {
                printf("题8  E(%d,%d) 第一次入队时，累计入队过 %d 个格子\n", er, ec, enq);
                printf("     入队顺序：");
                for (size_t i = 0; i < order.size(); i++) printf("%s ", order[i].c_str());
                printf("\n");
                return;
            }
        }
    }
    printf("题8  E 不可达（不该发生）\n");
}

static int gcd_self(int a,int b){while(b){int t=a%b;a=b;b=t;}return a;}

// ---------- 题 9：1<=n<=100 且 gcd(n,60)=6 的 n ----------
static void q9() {
    vector<int> hit;
    for (int n = 1; n <= 100; n++)
        if (gcd_self(n, 60) == 6) hit.push_back(n);
    printf("题9  共 %d 个：", (int)hit.size());
    for (int v : hit) printf("%d ", v);
    printf("\n");
}

// ---------- 题 10：面值 1/4/6 凑 9 元最少几枚 ----------
static void q10() {
    const int coin[3] = {1, 4, 6};
    int dp[10];
    fill(dp, dp + 10, 1e9);
    dp[0] = 0;
    for (int i = 1; i <= 9; i++)
        for (int k = 0; k < 3; k++)
            if (i >= coin[k]) dp[i] = min(dp[i], dp[i - coin[k]] + 1);
    printf("题10 凑 9 元最少 %d 枚\n", dp[9]);
    printf("     贪心（每次先拿最大的）会给：9-6=3 再 3 枚 1 元 = 4 枚，比最优多 1 枚\n");
}

// ---------- 题 11：指针改写数组 ----------
static void q11() {
    int a[5] = {1, 3, 5, 7, 9};
    int* p = a + 2;
    *(p - 1) = p[0] + p[2];
    p[1] = *(a + 1) - a[0];
    printf("题11 输出 %d,%d\n", a[1], a[3]);
    printf("     改后整个数组 =");
    for (int i = 0; i < 5; i++) printf(" %d", a[i]);
    printf("\n");
}

// ---------- 题 12：1000 个元素二分查找最坏比较次数 ----------
static void q12() {
    int n = 1000;
    vector<int> a(n);
    iota(a.begin(), a.end(), 1);
    int worst = 0, worstKey = 0;
    auto cmp = [&](int key) {  // 卷面最常见的 lower_bound 式二分，逐次计比较
        int l = 1, r = n, c = 0;
        while (l <= r) {
            int mid = (l + r) / 2;
            c++;
            if (a[mid - 1] == key) return c;
            if (a[mid - 1] < key) l = mid + 1; else r = mid - 1;
        }
        return c;
    };
    for (int key = 0; key <= n + 1; key++) {  // 含查不到的哨兵值
        int c = cmp(key);
        if (c > worst) { worst = c; worstKey = key; }
    }
    printf("题12 最坏比较 %d 次（例如查找 %d）；理论 floor(log2(1000))+1 = %d\n", worst, worstKey, 10);
}

// ---------- 题 13：s[i]=3i^2+i，求 a[10] ----------
static void q13() {
    auto s = [](int i) { return 3 * i * i + i; };
    printf("题13 s(10)=%d s(9)=%d a[10]=s(10)-s(9)=%d\n", s(10), s(9), s(10) - s(9));
}

// ---------- 题 14：数轴 7 个点到某整数点距离和最小 ----------
static void q14() {
    vector<int> pt = {1, 3, 4, 7, 10, 15, 20};
    long long best = (long long)4e18;
    int bx = 0;
    for (int x = -5; x <= 30; x++) {
        long long sum = 0;
        for (int v : pt) sum += abs(x - v);
        if (sum < best) { best = sum; bx = x; }
    }
    printf("题14 最优整数点 x=%d，距离和 %lld\n", bx, best);
}

// ---------- 题 15：10 个顶点，4 个度 3 其余度 4，边数 ----------
static void q15() {
    int degsum = 4 * 3 + 6 * 4;
    printf("题15 度数和 = 4*3 + 6*4 = %d，边数 = %d / 2 = %d\n", degsum, degsum, degsum / 2);
}

int main() {
    q1();
    q2();
    q3();
    q4();
    q5();
    q6();
    q7();
    q8();
    q9();
    q10();
    q11();
    q12();
    q13();
    q14();
    q15();
    return 0;
}
