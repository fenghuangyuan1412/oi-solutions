// P1706 全排列问题 —— 讲课用代码（DFS + vis[] 回溯）
//
// 编译（本机有两套 MinGW 路径冲突，-static 必须加，否则运行时段错误）：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//
// 这版代码的目标不是"最短"，而是让学生在草稿纸上照着画出递归树，
// 所以刻意用全局数组 + 显式撤销，把"做选择 / 递归 / 撤回选择"三步摊开写。
#include <iostream>
#include <iomanip>          // setw 在这里；漏掉这行编译直接报错，是初学者第一坑
using namespace std;

int n;
int a[12];                  // a[d] = 递归第 d 层（即排列的第 d 个位置）填的数字
bool vis[12];               // vis[x] = 数字 x 此刻是不是已经排在序列里了

// d：现在要给第 d 个位置填数（下标从 0 开始数，所以 d == n 就是"n 个位置全填满"）
void dfs(int d) {
    if (d == n) {                                   // 递归边界：位置填满，这就是一个完整排列
        for (int i = 0; i < n; i++) cout << setw(5) << a[i];   // 场宽 5 是题面硬性要求，判题按它比对
        cout << '\n';                              // 用 '\n' 而不是 endl：n=9 实测 endl 版 574~588ms，本版 157~165ms
        return;                                    // 结算完必须返回，否则下面的循环会继续往下填
    }

    // 排列的灵魂在这一行：每一层都从 1 试到 n。
    // 为什么能保证字典序？因为"本层数字小的分支"先被完整走完，所以输出天然按字典序排列。
    for (int x = 1; x <= n; x++) {
        if (vis[x]) continue;                       // 这个数已经在序列里，跳过 —— vis[] 就是排列的去重器
        a[d] = x;                                   // ① 做了什么：把 x 放到第 d 个位置
        vis[x] = true;                              //    登记：x 已被占用
        dfs(d + 1);                                 // ② 往下递归：去填第 d+1 个位置
        vis[x] = false;                             // ③ 撤销什么：把 x 的登记抹掉，回到"本层还没试 x"的状态
        // 注意 a[d] 不用清 0：下一层循环会直接覆盖它；但 vis[] 必须清，否则同一条分支之后 x 永远"被用过"
    }
}

int main() {
    ios::sync_with_stdio(false);   // n = 9 要输出 362880 行、约 3.3e6 次 <<，
    cin.tie(nullptr);              // 关掉 iostream 与 stdio 的同步是"格式对但超时"的解药
    cin >> n;                      // 题面只给一个整数，没有多组数据
    dfs(0);                        // 从第 0 个位置开始填
    return 0;
}
