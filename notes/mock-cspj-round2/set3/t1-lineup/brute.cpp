#include <bits/stdc++.h>
using namespace std;

// 暴力：真的把队伍一遍遍重排（只对小规模有意义）
int main() {
    int n, x;
    scanf("%d %d", &n, &x);
    vector<int> q(n);
    for (int i = 0; i < n; i++) q[i] = i + 1;
    int rounds = 0, mine = 0;
    while (!q.empty()) {
        rounds++;
        vector<int> rest;
        bool got = false;
        for (int i = 0; i < (int)q.size(); i++) {
            if (i % 3 == 0) {  // 第 1,4,7,... 个位置（下标 0,3,6,...）
                if (q[i] == x) got = true;
            } else {
                rest.push_back(q[i]);
            }
        }
        if (got) mine = rounds;
        q = rest;
    }
    printf("%d %d\n", rounds, mine);
    return 0;
}
