// P1443 马的遍历 —— BFS 求起点到每格最少步数
// 编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//
// 核心思想：马每一步"代价相同"（都按 1 步），所以从起点一层一层扩散出去的
// BFS 顺序天然是"按步数从小到大"，第一次碰到某个格子时的一定是最少步数。
// 这一点是整道题正确性的根基，见 README「正确性」。
#include <cstdio>
#include <queue>
using namespace std;

// 马走"日"字：8 个方向一个都不能少。写成 4 个是最常见的错（那是车的走法的一半）。
// 注意 dx/dy 配对：(±1,±2) 与 (±2,±1)，横竖两种"日"字各 4 个。
const int DX[8] = {1, 2, 2, 1, -1, -2, -2, -1};
const int DY[8] = {2, 1, -1, -2, -2, -1, 1, 2};

int n, m, sx, sy;
int distArr[405][405];   // 同时充当 vis：-1 表示"还没访问过 / 不可达"

int main() {
    scanf("%d %d %d %d", &n, &m, &sx, &sy);

    // 初值必须是 -1 而不是 0：
    //  1) 起点会被写成 0，未访问格保持 -1，输出时天然就是"不可达 -1"；
    //  2) 如果初值 0，"未访问"和"起点 0 步"无法区分，BFS 会把所有空格当成已访问。
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            distArr[i][j] = -1;

    queue<pair<int, int> > q;            // C++14 里 ">>" 也可，写开是照顾老编译器习惯
    distArr[sx][sy] = 0;                 // 入队前就打标记！若出队时才打，同一格会被重复入队爆内存
    q.push(make_pair(sx, sy));

    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop();                          // front/pop 分开取，写法更直观
        int x = cur.first, y = cur.second;
        for (int k = 0; k < 8; k++) {
            int nx = x + DX[k], ny = y + DY[k];
            // 先判界再取数组：C++ 不做下标检查，越界读到的是垃圾值（且 distArr 只开到 405）
            if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
            if (distArr[nx][ny] != -1) continue;   // 已访问（或就是起点）直接跳过
            distArr[nx][ny] = distArr[x][y] + 1;   // 层数 +1，就是"再多走一步"
            q.push(make_pair(nx, ny));
        }
    }

    // 输出 160000 个数（400x400），体量约 0.5~1 MB。
    // printf 自带缓冲，直接够用；若用 cout 必须 ios::sync_with_stdio(false) 才不会拖慢。
    // 洛谷 2022-08 后不要求场宽，空格分隔即可判对。
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++)
            printf("%d ", distArr[i][j]);  // 行末多余空格判定接受（题面明示"空格或场宽均可"）
        printf("\n");
    }
    return 0;
}
