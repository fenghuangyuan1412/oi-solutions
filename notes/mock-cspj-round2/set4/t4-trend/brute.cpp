#include <bits/stdc++.h>
using namespace std;

int p[25];

// 暴力：n ≤ 18，枚举所有非空子集，逐个检查"严格上升 + 间隔 ≤ d"
int main() {
    int n, d;
    scanf("%d %d", &n, &d);
    for (int i = 0; i < n; i++) scanf("%d", &p[i]);
    int best = 0, cnt = 0;
    for (int mask = 1; mask < (1 << n); mask++) {
        vector<int> idx;
        for (int i = 0; i < n; i++) if (mask >> i & 1) idx.push_back(i);
        bool ok = true;
        for (int j = 1; j < (int)idx.size(); j++) {
            if (idx[j] - idx[j - 1] > d || p[idx[j]] <= p[idx[j - 1]]) ok = false;
        }
        if (!ok) continue;
        int len = (int)idx.size();
        if (len > best) {
            best = len;
            cnt = 1;
        } else if (len == best) cnt++;
    }
    printf("%d %d\n", best, cnt);
    return 0;
}
