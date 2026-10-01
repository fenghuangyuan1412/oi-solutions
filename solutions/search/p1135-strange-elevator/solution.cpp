// P1135 奇怪的电梯 —— 把楼层当状态，BFS 求最少按键次数
// 编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//
// 建模是本题的全部难点：楼层只有 200 个，"按一次上/下按钮"从一个楼层跳到
// 另一个楼层，恰好像一张图 —— 每个点固定 2 条出边（+K[i] 和 -K[i]），边权都是 1。
// 边权全 1 的最短路 = BFS 层数，不需要 Dijkstra（那是带权图用的）。
#include <cstdio>
#include <queue>
using namespace std;

int n, a, b;
int k[205];
int distArr[205];   // -1 = 未访问，兼当 vis 与"不可达"输出

int main() {
    scanf("%d %d %d", &n, &a, &b);
    for (int i = 1; i <= n; i++) scanf("%d", &k[i]);
    for (int i = 1; i <= n; i++) distArr[i] = -1;

    // A == B 特判其实可以省：起点 dist 记 0，BFS 结束时直接输出即可，天然给 0。
    // 但课堂上建议显式想一遍"0 次按键"是否合法 —— 题面问"至少要按几次"，按 0 次就是答案。
    queue<int> q;
    distArr[a] = 0;                 // 入队前打标记，保证每个楼层最多进队一次
    q.push(a);

    while (!q.empty()) {
        int x = q.front(); q.pop();
        // "按钮失灵"的正确实现 = 只在不越界时才有这条边。
        // 千万不要先算出 nx 再访问 distArr[nx]：C++ 下标越界不报错，读到垃圾值直接 WA/RE。
        int up = x + k[x];
        if (up <= n && distArr[up] == -1) {
            distArr[up] = distArr[x] + 1;
            q.push(up);
        }
        int down = x - k[x];
        if (down >= 1 && distArr[down] == -1) {
            distArr[down] = distArr[x] + 1;
            q.push(down);
        }
        // k[x]==0 时 up==down==x，靠 distArr[x]!=-1 自动跳过，不会死循环；
        // 但要想清楚为什么不会：标记在入队时打，而不是弹出来才打。
    }

    // 目标是"到 B 的最少次数"，不必跑完整个 BFS 也能在碰到 B 时立刻退出；
    // 这里为了讲清"dist 数组就是标准答案表"，让它自然跑完（N<=200，无所谓）。
    printf("%d\n", distArr[b]);     // 不可达时保持 -1，正好是题面要求的输出
    return 0;
}
