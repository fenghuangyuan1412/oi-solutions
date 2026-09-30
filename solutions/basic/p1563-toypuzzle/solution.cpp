#include <bits/stdc++.h>
using namespace std;

const int N = 100005;
int dir[N];              // 0 = 朝圈内，1 = 朝圈外
string job[N];           // 职业名，可能长短不一，必须用 string 存

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; i++) cin >> dir[i] >> job[i];   // 按逆时针顺序给出 ⇒ 下标 +1 就是逆时针挪一格

    int cur = 0;         // 从第 1 个小人出发（0 号）
    for (int i = 0; i < m; i++) {
        int a, s;
        cin >> a >> s;   // a=0 向左数 s 个人，a=1 向右数 s 个人；"数 s 个"= 走 s 步

        // 朝向决定左右：
        //   朝内 ⇒ 左边是顺时针(下标 -1)，右边是逆时针(下标 +1)
        //   朝外 ⇒ 左边是逆时针(下标 +1)，右边是顺时针(下标 -1)
        // 注意取的是【当前所在小人】的朝向，不是下一条指令的朝向 —— 本题最大的坑。
        int step;
        if (dir[cur] == 0) step = (a == 0 ? -1 : 1);
        else               step = (a == 0 ? 1 : -1);

        // 一次算完，不要 for (j=1..s) 一步步挪：
        // 一步步挪是 O(Σs) ≤ O(nm) = 1e10，只能过 n≤20 的前 16 个测试点（80 分）。
        cur = ((cur + step * s) % n + n) % n;   // C++ 负数取模可能为负，必须 +n 再 %n
    }

    cout << job[cur] << '\n';
    return 0;
}
