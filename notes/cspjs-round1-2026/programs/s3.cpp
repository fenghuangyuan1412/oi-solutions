// S3 — 单调栈求下一个更大元素
// 考察：栈操作顺序、严格小于 vs 小于等于、均摊复杂度
#include <iostream>
#include <stack>
using namespace std;

int main() {
    int a[7] = {5, 3, 8, 1, 6, 2, 4};
    int n = 7, res[7];
    stack<int> st;

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.top()] < a[i]) {
            res[st.top()] = a[i];
            st.pop();
        }
        st.push(i);
    }
    // 栈里剩下的是右侧没有更大元素的
    while (!st.empty()) {
        res[st.top()] = -1;
        st.pop();
    }
    for (int i = 0; i < n; ++i) cout << res[i] << " ";
    cout << "\n";
    return 0;
}
// a = {5, 3, 8, 1, 6, 2, 4}
// res: 5->8, 3->8, 8->-1, 1->6, 6->-1, 2->4, 4->-1
// 预期输出：8 8 -1 6 -1 4 -1
