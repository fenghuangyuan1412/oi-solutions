#include <bits/stdc++.h>
using namespace std;

// 为什么开在全局：2×10^6 个 int ≈ 8MB。
// 局部数组放在栈上，Windows 默认栈 1MB、Linux 默认 8MB，直接爆栈 RE；
// 全局数组放在静态存储区（BSS），只受 125MB 内存限制约束，8MB 绰绰有余。
// 长度多开 5 格：下标从 1 用到 n，写成 a[2000000] 时 a[2000000] 就越界了。
const int MAXN = 2000005;
int a[MAXN];

int main() {
    // 读入量 = 2×10^6 + 10^5 个数，输出 10^5 行，时限 1s。
    // cin 默认与 C 的 stdio 同步（每个数都要冲一次缓冲区），关掉才能跟上。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;  // 第一行是"学生个数 n、询问次数 m"，顺序读反后面全错

    // 进教室的顺序就是下标的顺序：第 i 个进来的同学存进 a[i]
    for (int i = 1; i <= n; ++i) cin >> a[i];

    for (int q = 1; q <= m; ++q) {
        int i;
        cin >> i;
        // 题目问"第 i 个人"，而"第 i 个人"当初就写在 a[i]：一次内存访问，无需查找
        cout << a[i] << '\n';  // 用 '\n' 不用 endl：endl 每行都 flush 一次
    }
    return 0;
}
