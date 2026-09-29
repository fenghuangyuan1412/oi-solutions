// S2 — 组合数递归（无记忆化）及调用次数
// 考察：递归树、叶节点数 = 组合数值、调用次数公式
#include <iostream>
using namespace std;

int calls = 0;

int C(int n, int m) {
    ++calls;
    if (m == 0 || n == m) return 1;   // 叶子
    return C(n - 1, m) + C(n - 1, m - 1);
}

int main() {
    int ans = C(6, 3);
    cout << ans << " " << calls << "\n";
    return 0;
}
// C(6,3) = 20
// 调用次数 = 2*C(6,3)-1 = 39（满二叉树：内部节点=叶子-1，总=2*叶子-1）
// 预期输出：20 39
