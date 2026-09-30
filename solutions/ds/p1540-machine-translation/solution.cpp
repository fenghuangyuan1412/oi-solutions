// P1540 [NOIP 2010 提高组] 机器翻译
// 编译：g++ -static -O2 -std=c++14 solution.cpp -o sol.exe
//
// 模型：内存就是一个 FIFO 队列 —— 淘汰的永远是"最早进入内存"的那个单词。
// 配套一个 bool in[] 记录"某个单词现在在不在内存里"，把 O(M) 的查找降到 O(1)。

#include <cstdio>
#include <queue>
using namespace std;

// 题面：单词是"大小不超过 1000 的非负整数"，也就是 0..1000 都合法。
// 下标 0 这一格必须存在，所以数组至少开到 1001；这里开 1005 留一点余量。
bool in[1005];

int main() {
    // 注意读入顺序：先 M（内存容量）后 N（文章长度），和习惯写的 n m 正好相反。
    int M, N;
    if (scanf("%d%d", &M, &N) != 2) return 0;

    queue<int> q;   // q.front() 是当前内存中"最早进入"的单词
    int ans = 0;

    for (int idx = 0; idx < N; ++idx) {
        int w;
        scanf("%d", &w);

        // 命中：内存里已经有这个单词，直接查出来用。
        // 关键：命中时【什么都不做】。队列顺序、in[] 标记都不动 ——
        // 因为淘汰规则只看"什么时候进入内存"，不看"什么时候被访问"。
        if (in[w]) continue;

        // 未命中：必须去外存查词典，次数 +1。
        ++ans;

        // 先放入队尾（新进入内存的单词一定是最新的），再判是否超容量。
        q.push(w);
        in[w] = true;

        // 超容量说明放进来的这一刻内存原本是满的，需要腾一个单元。
        // 腾的是队头 = 最早进入的那个，这就是 FIFO。
        if ((int)q.size() > M) {
            // 清标记必须在 pop() 之前：pop() 之后 front() 已经是"下一个"单词了，
            // 那时再 in[q.front()] = false 就会把无辜单词的标记抹掉。
            in[q.front()] = false;
            q.pop();
        }
    }

    printf("%d\n", ans);
    return 0;
}
