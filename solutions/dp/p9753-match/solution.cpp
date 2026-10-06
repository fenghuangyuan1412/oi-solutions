/*
 * P9753 [CSP-S 2023] 消消乐 —— 正解：前缀栈（约简唯一化）+ 相同前缀配对计数，O(26n)
 * 详见同目录 README.md「二、正解思路」
 * 编译：g++ -static -O2 -std=c++14 solution.cpp -o out.exe
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    if (!(cin >> n)) return 0;
    cin >> s;

    /* 第 1 步：一趟扫描，把每个前缀 s[1..i] 用"栈"约简到底（栈顶和新字符相同就弹栈）。
     * 约简规则 aa->e 只作用在相邻两位上，且它是 confluent 的：任意删法得到的最简串都唯一。
     * 所以：子串可消除 <=> 它约简后为空 <=> 它两端的前缀约简结果相同（群里的除法）。
     *
     * 栈的内容可能很长，不能直接比较。但所有出现过的栈内容构成一棵树：
     *   节点 = 一种栈内容，父亲的边 = 弹出最后一个字符，儿子 = 在末尾加一个字符。
     * 于是给每种栈内容发一个小整数编号：新内容 = (父编号 u, 新字符 c)，
     * 同一个 (u, c) 永远对应同一个编号 —— 查"u 的 c 儿子"存在与否即可，不用任何哈希。 */
    vector<int> firstChild(n + 2, 0);   // 节点 -> 第一个儿子
    vector<int> nextSib(n + 2, 0);      // 儿子 -> 下一个兄弟（每个节点最多 26 个兄弟，暴力扫）
    vector<char> edgeChar(n + 2, 0);    // 这条边加的字符
    vector<int> stk(n + 2, 0);          // 当前栈里的字符
    vector<int> idAt(n + 2, 0);         // idAt[d] = 当前栈前 d 个字符这个内容的编号
    vector<int> seen(n + 2, 0);         // 某个编号在前面出现过几次

    const int EMPTY = 1;                // 编号 1 = 空栈
    int nodes = 1;                      // 已用编号数
    int top = 0;                        // 当前栈深
    idAt[0] = EMPTY;

    long long ans = 0;
    seen[EMPTY] = 1;                    // 空前缀 s[1..0] 也要计入，见 README 易错点 2

    for (int i = 1; i <= n; ++i) {
        char c = s[i - 1];
        if (top >= 1 && stk[top] == c) {
            --top;                      // 和栈顶相同 -> 两个一起消掉，退回上一层
        } else {
            stk[++top] = c;
            int u = idAt[top - 1];      // 父节点编号
            int v = 0;
            for (int e = firstChild[u]; e; e = nextSib[e])   // 最多 26 次比较
                if (edgeChar[e] == c) { v = e; break; }
            if (v == 0) {               // (u, c) 第一次出现 -> 开新节点
                v = ++nodes;
                edgeChar[v] = c;
                nextSib[v] = firstChild[u];
                firstChild[u] = v;
            }
            idAt[top] = v;
        }
        /* 第 2 步：前面有多少个前缀和当前栈内容一模一样，就有多少个以 i 结尾的可消除子串 */
        int st = idAt[top];
        ans += seen[st];
        ++seen[st];
    }

    cout << ans << "\n";
    return 0;
}
