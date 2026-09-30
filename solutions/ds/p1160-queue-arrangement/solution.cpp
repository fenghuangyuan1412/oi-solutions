// P1160 队列安排
// 编译：g++ -static -O2 -std=c++14 solution.cpp -o sol.exe
//
// 模型：双向链表（数组模拟）。同学编号本身就是节点号，所以"定位到 k 号同学"是 O(1)，
// 插入/删除只改几根指针，也是 O(1)，整题 O(N+M)。
// 用 0 号哨兵把链表接成环：R[0] 是队头、L[0] 是队尾，这样"插到队头左边""删掉队头"
// 都不用特判边界。

#include <cstdio>

const int MAXN = 100005;
int L[MAXN], R[MAXN];   // L[i]/R[i] = i 的左/右邻居编号；0 是哨兵
bool gone[MAXN];        // gone[i] = i 已经被移出队列（重复删除指令要忽略）

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    L[0] = R[0] = 0;    // 空环：哨兵自己指向自己

    // 第 1 步：先把 1 号放进队列。挂到哨兵旁边即可（此时他是队头也是队尾）
    int first = 1;
    L[first] = R[first] = 0;
    R[0] = first;
    L[0] = first;

    // 第 2 步：2..N 依次插到某个已在队列中的同学 k 的左/右边
    for (int i = 2; i <= n; ++i) {
        int k, p;
        scanf("%d%d", &k, &p);
        if (p == 0) {
            // 插到 k 的左边，即塞进 L[k] 与 k 之间。
            // 顺序要点：先把 x=L[k] 取出来存好，再动手改 L[k]，否则原来的左邻居会丢。
            int x = L[k];
            R[x] = i; L[i] = x;
            L[k] = i; R[i] = k;
        } else {
            // 插到 k 的右边，即塞进 k 与 R[k] 之间
            int y = R[k];
            L[y] = i; R[i] = y;
            R[k] = i; L[i] = k;
        }
    }

    // 第 3 步：M 次移出。已不在队列中的指令必须忽略 ——
    // 没有 gone[] 标记时对同一人删两次，会把"已经改过的"邻居指针再改一遍，链表当场断裂。
    int m;
    scanf("%d", &m);
    while (m--) {
        int x;
        scanf("%d", &x);
        if (gone[x]) continue;      // 忽略重复删除
        gone[x] = true;
        R[L[x]] = R[x];             // 让左右邻居直接牵手，x 就离场了
        L[R[x]] = L[x];
    }

    // 从哨兵的右边（真正的队头）沿 R 走，走回哨兵就结束
    bool head = true;
    for (int x = R[0]; x != 0; x = R[x]) {
        if (!head) putchar(' ');
        head = false;
        printf("%d", x);
    }
    putchar('\n');
    return 0;
}
