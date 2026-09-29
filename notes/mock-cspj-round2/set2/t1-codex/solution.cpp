#include <bits/stdc++.h>
using namespace std;

const int M = 1000005;
bool have[M];

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int distinct = 0;
    for (int i = 1; i <= n; i++) {
        int c;
        scanf("%d", &c);
        if (!have[c]) {  // 第一次抽到才算"点亮图鉴"
            have[c] = true;
            distinct++;
        }
    }
    printf("%d\n", m - distinct);
    return 0;
}
