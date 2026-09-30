#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int M, N;
    cin >> M >> N;        // 注意顺序：先内存单元数 M，再文章长度 N（读反就全盘皆错）

    // 内存就是一个 FIFO 队列：队头是"最早进入"的单词，队尾是最新的
    queue<int> mem;
    set<int> inMem;       // 判断"这个词在不在内存里"。M ≤ 100，用数组/线性扫也一样对

    int lookup = 0;       // 查词典次数 = 未命中次数
    for (int i = 0; i < N; i++) {
        int w;
        cin >> w;
        if (inMem.count(w)) {
            continue;     // 命中：直接用，内存内容一个字都不动（这是 FIFO，不是 LRU！）
        }
        lookup++;         // 未命中：查一次外存
        if ((int)mem.size() == M) {   // 只有装满 M 个才需要淘汰；"不超过 M-1"时直接填空位
            inMem.erase(mem.front());
            mem.pop();
        }
        mem.push(w);
        inMem.insert(w);
    }

    cout << lookup << '\n';
    return 0;
}
