#include <bits/stdc++.h>
using namespace std;

// 判断是否为左括号
bool isOpen(char c) { return c == '(' || c == '['; }
// 判断左括号 a 与右括号 b 是否为同一类型（小配小、中配中，不能交叉互配）
bool sameType(char a, char b) {
    return (a == '(' && b == ')') || (a == '[' && b == ']');
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    // 用 getline 读整行：题面允许 |s| 可能为 0（空行），cin >> s 会读不到空串
    getline(cin, s);
    int n = (int)s.size();

    vector<int> st;                  // 栈：存"尚未匹配的左括号"在原串中的下标
    vector<bool> matched(n, false);  // matched[i]=true 表示 s[i] 已成功配对

    for (int i = 0; i < n; ++i) {
        char c = s[i];
        if (isOpen(c)) {
            // 左括号此刻一定未匹配，压入栈；下标递增保证栈顶就是"最近的"
            st.push_back(i);
        } else {
            // 右括号：题面只看"左侧最近的未匹配左括号"，即栈顶
            // 关键区别 —— 只有类型匹配才弹栈；类型不符或栈空时，右括号作废，
            // 而栈顶那枚左括号【留在栈里】（它还没被匹配，后面仍可能被别的右括号配上）
            if (!st.empty() && sameType(s[st.back()], c)) {
                matched[st.back()] = true;
                matched[i] = true;
                st.pop_back();
            }
            // 否则：本右括号保持未匹配，栈不做任何操作
        }
    }

    // 扫描结束后：栈里剩下的都是未匹配的左括号；配对失败的右括号不在栈里但 matched=false
    // 逐字符按原顺序补全：不能边扫边输出，因为"是否未匹配"要扫完才全部确定
    string ans;
    for (int i = 0; i < n; ++i) {
        char c = s[i];
        if (matched[i]) {
            ans += c;                                  // 已配对的字符原样保留
        } else if (isOpen(c)) {
            ans += c;                                   // 未匹配左括号：在右边补对应右括号
            ans += (c == '(' ? ')' : ']');
        } else {
            ans += (c == ')' ? '(' : '[');             // 未匹配右括号：在左边补对应左括号
            ans += c;
        }
    }
    cout << ans << '\n';
    return 0;
}
