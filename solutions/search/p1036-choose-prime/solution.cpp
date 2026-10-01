// P1036 [NOIP 2002 普及组] 选数 —— 讲课用代码（枚举组合 + 判素数，两层各司其职）
//
// 编译（本机两套 MinGW 路径冲突，-static 必加）：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//
// 教学定位：这题 = P1157 的组合枚举 + 一个"判定函数"。
// 把"怎么把所有选法枚举出来"和"怎么判断一个和是不是素数"分开写，各自都不难。
#include <iostream>
using namespace std;

int n, k;
long long x[25];        // xi <= 5e6，用 long long 存着最省心（下面解释为什么 int 其实也够）
int ans = 0;            // 和为素数的选法种数

// 试除判素：只看有没有 2..sqrt(v) 之间的因子。
// 两个必须写对的地方：① v < 2 直接不是素数（1 不是素数！初学者最爱把它当素数）；
// ② 循环条件写成 i * i <= v，而不是 i <= sqrt(v)：sqrt 是浮点函数，边界上会因为精度抖一下。
bool isPrime(long long v) {
    if (v < 2) return false;                     // 0 和 1 都不是素数；负数用不到，但这条也顺手挡住了
    for (long long i = 2; i * i <= v; i++)       // i 到 sqrt(v) 就够：因子是成对出现的，
        if (v % i == 0) return false;            // 若 v = a*b，则必有一个 <= sqrt(v)
    return true;                                 // 全程没找到因子 —— 包括 v = 2 时循环一次都不进，直接 true
}

// start：本层及之后能选的最小下标（保证组合内下标递增，天然不重复、天然字典序）
// dep  ：已经选了几个数
// sum  ：这几个数的和 —— 一路带着走，就不用回到终点再重新累加
void dfs(int start, int dep, long long sum) {
    if (dep == k) {                              // 选够 k 个了，这一种的 and 判定交给 isPrime
        if (isPrime(sum)) ans++;
        return;
    }
    for (int i = start; i < n; i++) {            // 从上一个下标 + 1 开始，所以 (i,j) 与 (j,i) 只会出现一次
        dfs(i + 1, dep + 1, sum + x[i]);         // 做选择 = 传参，撤销 = 参数是局部变量，返回自动消失
        // 这里不需要 vis[]：下标只朝一个方向走，不可能重复使用同一个数。
        // 想加剪枝也很简单：i <= n - (k - dep)，"剩下的数刚好够选"，见 README 常见变式。
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> k;
    for (int i = 0; i < n; i++) cin >> x[i];
    dfs(0, 0, 0);                                // 下标从 0 开始，已选 0 个，和为 0
    cout << ans << '\n';                         // 只输出一个整数，没有格式陷阱
    return 0;
}

// 关于数据范围的一笔账（讲课要算给学生听）：
// k < n <= 20 ⇒ k 最大 19，和的最大可能值 = 19 * 5e6 = 9.5e7，int（约 2.1e9）装得下，
// 但判素循环里的 i * i 若用 int 且在更大题目里会爆，所以本代码统一用 long long，
// 顺便让学生养成"乘一下就先看看会不会溢出"的习惯。
// 组合数：k=19 时只有 C(20,19)=20 种；最多的是 k=10 的 C(20,10)=184756 种。
// 判素成本：最坏每次约 sqrt(9.5e7) ≈ 9747 轮（本写法逐个整数试，不是只试素数），
// 悲观上界 184756 * 9747 ≈ 1.8e9 看着吓人，实际绝大多数和在第一两轮就被小因子（2、3）筛掉：
// 本机实测 n=20、k=10、x_i 随机 <=5e6 的极限数据只要 458~498 ms（洛谷时限 1500ms），
// k=19 那档（和接近 9.4e7、要试除到 9700 多）反而只要 7~9 ms，因为组合数只有 20 个。
