// P14358 [CSP-J 2025] 座位
// 原作者解法（逻辑经对拍验证正确），此处保留原样并补充逐段说明。
//
// 核心思想：蛇形是按"列"走的，每列恰好 n 个人。
// 只要知道小 R 的降序名次 k，列号 c = ceil(k/n)，行号由 c 的奇偶决定。

#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct node {
    int id, x;          // id = 读入顺序（小 R 为 1），x = 成绩
} ar[100050];

bool cmp(node a, node b) {
    return a.x > b.x;   // 降序：成绩高的排前面
}

int main() {
    int n, m, ans;      // n 行数、m 列数、ans 将存放小 R 的名次 k
    cin >> n >> m;

    for (int i = 1; i <= n * m; i++) {
        cin >> ar[i].x;
        ar[i].id = i;   // 打标记：i == 1 就是小 R，排序后要把它找回来
    }

    // 排序后 ar[i].x 从大到小，下标 i 就是"第几名"
    sort(ar + 1, ar + 1 + n * m, cmp);

    // 定位小 R 的名次：谁带着 id == 1，谁就是第几名
    for (int i = 1; i <= n * m; i++) {
        if (ar[i].id == 1) {
            ans = i;
        }
    }

    int a1 = ceil(ans * 1.0 / n);   // 列号 c：前 c-1 列共 (c-1)*n 人
    if (a1 & 1) {
        // 奇数列自上而下填充，行号 = 本列内的第几个人
        cout << a1 << " " << ans - (a1 - 1) * n;
    } else {
        // 偶数列自下而上填充，行号需要翻转：r = n - (k-(c-1)n) + 1 = c*n - k + 1
        cout << a1 << " " << a1 * n - ans + 1;
    }
    return 0;
}

/* 考场上更稳的等价写法（三处加固）：
 *   int ans = -1;                    // 显式初始化
 *   ...
 *   if (ar[i].id == 1) { ans = i; break; }
 *   int c = (ans + n - 1) / n;       // 纯整数上取整，不碰浮点
 *   int r = (c & 1) ? ans - (c - 1) * n : c * n - ans + 1;
 *   cout << c << " " << r;
 *
 * 也可以完全不排序，直接数"有多少人成绩比 a_1 高"得到名次，O(nm)：
 *   int k = 1;
 *   for (int i = 2; i <= n * m; i++) if (ar[i].x > ar[1].x) k++;
 */
