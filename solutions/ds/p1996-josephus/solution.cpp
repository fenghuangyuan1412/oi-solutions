#include <bits/stdc++.h>
using namespace std;

int main() {
    // 数据只有 1..100，同步开关本题不影响正确性，但这是 OI 的固定开头，
    // 一旦哪天 n 上到 10^6，少这两行就是 1s 与 10s 的差别。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    // 把"围成一圈"摊平成一条队列：队头 = 下一个要报数的人。
    // 圈的"往后走"用"出队"表示，圈的"绕回开头"用"入队到队尾"表示。
    queue<int> q;
    for (int i = 1; i <= n; ++i) q.push(i);

    // 注意循环要做满 n 轮：本题要输出全部出圈顺序，不是只留最后一个人。
    for (int k = 1; k <= n; ++k) {
        // 报 1 ~ m-1 的人暂时不出圈，依次挪到队尾，等下一轮再报。
        // 这里是 m-1 次而不是 m 次：报 m 的那个人必须留在队头等着被弹出。
        for (int t = 1; t < m; ++t) {
            q.push(q.front());
            q.pop();
        }
        // 队列非空（第 k 轮开始时还剩 n-k+1 ≥ 1 人），front 就是报数为 m 的人
        if (k > 1) cout << ' ';  // 第一个数前面不多打空格
        cout << q.front();
        q.pop();  // 弹出＝出圈；下一轮从他下一个人开始报 1，正好是新队头
    }
    cout << '\n';
    return 0;
}
