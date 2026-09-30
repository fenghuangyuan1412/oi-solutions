#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    while (q--) {
        int n;
        cin >> n;
        vector<int> pushed(n), poped(n);
        for (int i = 0; i < n; ++i) cin >> pushed[i];
        for (int i = 0; i < n; ++i) cin >> poped[i];

        // 用数组手写栈：比 std::stack(deque) 快，且能直接读栈顶，O(1)
        vector<int> st;
        st.reserve(n);
        int j = 0;  // j 指向 poped 中"下一个要弹出"的目标位置

        // 入栈顺序被 pushed 钉死，我们唯一能决定的是"什么时候弹"
        // 贪心：每压入一个元素，就尽可能把栈顶对齐目标的连续段全部弹掉
        for (int i = 0; i < n; ++i) {
            st.push_back(pushed[i]);
            // while 而非 if：一次压入后可能连续弹出多个（如 pushed=1..5, poped=5 4 3 2 1）
            while (!st.empty() && j < n && st.back() == poped[j]) {
                st.pop_back();
                ++j;
            }
        }

        // 判定看"是否全部弹出"（j==n），而非"栈是否空"——j==n 更贴合题面语义
        cout << (j == n ? "Yes" : "No") << '\n';
    }
    return 0;
}
