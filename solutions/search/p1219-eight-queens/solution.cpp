// P1219 [USACO1.5] 八皇后 Checker Challenge —— 按行回溯 + 三组标记数组剪枝
// 编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//
// 降维思路：n 个棋子"每行有且只有一个"⇒ 索性按行放，第 i 行只决定列号。
// 这样解天然是一个长度为 n 的排列（题面要求的输出格式 2 4 6 1 3 5 也是这个意思），
// 搜索树从"选格子"(n^2 层) 降到"选列"(n 层)，规模指数级缩小。
#include <cstdio>

int n;
int col[15];          // col[i] = 第 i 行放的棋子所在列（存方案本身，输出要用）
bool usedCol[15];     // 列占用标记
bool d1[30];          // 副对角线（↗ 方向，行+列 相同）：i+j 范围 [2, 2n]，开 2n+1 足够
bool d2[30];          // 主对角线（↘ 方向，行-列 相同）：i-j 范围 [1-n, n-1]，加偏移 n 后 [1, 2n-1]
int total = 0;

// 为什么 d2 要加 n？i-j 最小可到 1-n（负数），C++ 负下标是未定义行为；
// 加 n 把它平移进 [1, 2n-1]，数组只要开 2n（这里 n<=13，直接开 30 省事）。
// 而 i+j 本身最小是 2，不会为负，不用平移 —— 两条对角线的区别要在黑板上讲清楚。

void printSolution() {
    total++;
    if (total <= 3) {                     // 只打印前 3 个解，但计数必须走完全部
        for (int i = 1; i <= n; i++)
            printf("%d%c", col[i], i == n ? '\n' : ' ');  // 数字间一个空格，行末不留空格
        // 注意：这里刻意写成 "行末无空格"。洛谷此题用严格比较，多一个尾空格可能 PE。
    }
}

// 按行递归：dfs(i) 负责给第 i 行选一个合法列。i > n 说明 1..n 行全放好了。
void dfs(int i) {
    if (i > n) { printSolution(); return; }
    for (int j = 1; j <= n; j++) {
        if (usedCol[j] || d1[i + j] || d2[i - j + n]) continue;  // 三重剪枝，全 O(1)
        col[i] = j;                                              // 记录选择：回溯题输出方案的标配
        usedCol[j] = d1[i + j] = d2[i - j + n] = true;           // 打标记
        dfs(i + 1);                                              // 进入下一行
        usedCol[j] = d1[i + j] = d2[i - j + n] = false;          // 撤销标记（回溯的灵魂）
    }
    // 列从小到大枚举 ⇒ 找到的方案天然按 col[1],col[2],... 字典序出现，不需要额外排序。
    // 这一点题面有要求（"解按字典顺序排列"），要在代码注释里点破。
}

int main() {
    scanf("%d", &n);
    dfs(1);
    printf("%d\n", total);      // 最后一行：解的总数（前 3 解只是"样本"，总数要求全数完）
    return 0;
}
