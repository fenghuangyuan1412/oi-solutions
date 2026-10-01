// P1157 组合的输出 —— 讲课用代码（DFS 枚举组合）
//
// 编译（本机两套 MinGW 路径冲突，-static 必加）：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//
// 和 P1706 全排列放在一起讲最好：那题每层从 1 试到 n、靠 vis[] 去重；
// 这题每层从"上一个数 + 1"试到 n，不需要 vis[]，而且顺序天然是字典序。
#include <iostream>
#include <iomanip>          // setw(3) 需要它，题面专门提醒过 —— 本题第一坑
using namespace std;

int n, r;
int a[25];                  // a[dep] = 组合里第 dep 个位置（从 0 数）放的数

// start：本层以及之后允许出现的最小数；dep：现在要确定组合里的第 dep 个数
void dfs(int start, int dep) {
    if (dep == r) {                                   // 递归边界：已经选够 r 个数，输出一个组合
        for (int i = 0; i < r; i++) cout << setw(3) << a[i];   // 每元素 3 场宽；最后一个是 a[r-1]，行尾不留空格
        cout << '\n';
        return;
    }

    // 组合的灵魂：循环起点是 start（= 上一层选的数 + 1），不是 1。
    // 于是 a[0] < a[1] < ... < a[r-1] 恒成立 —— 每个组合内部自动升序，
    // 不同组合之间也不可能重复（想重复就必须选同一个数两次，而一旦选了就只能往后走）。
    // 把这里的 start 改成 1，立刻退化成"排列"的错误写法：输出会多出一大片、且顺序不对。
    for (int x = start; x <= n; x++) {
        a[dep] = x;                                   // ① 做了什么：第 dep 个数取 x
        dfs(x + 1, dep + 1);                          // ② 往下递归：下一个数必须比 x 大，所以传 x + 1
        // ③ 撤销什么：只写了 a[dep]，下一层循环会覆盖它，没有 vis 要还原 —— 这正是组合比排列省心的地方
    }
}

int main() {
    ios::sync_with_stdio(false);       // n=20, r=10 要输出 184756 行，关掉同步稳一点
    cin.tie(nullptr);
    cin >> n >> r;                     // 题面第一行是 n r（样例 "5 3" 后面带一个空格，cin 会自动跳过）
    dfs(1, 0);                         // 最小可以从 1 开始选，目前一个数都没选
    // r == 0 时 dfs 第一件事就是命中边界，输出一个空行 —— 相当于"空组合只有一个"，
    // 洛谷数据 1 < n 且实际不卡 r=0，但把这个特判写进注释，考场上遇到就知道自己在输出什么
    return 0;
}
