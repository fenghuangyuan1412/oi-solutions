// fill2_tpl.cpp -- 洛谷 SCP-S1 2026 完善程序(2) 静态 Top Tree 的原样还原（五个空为 @@B1@@..@@B5@@）
// 官方答案: 39 D, 40 A, 41 D, 42 A, 43 C
#include <algorithm>
#include <iostream>
using namespace std;
const int N = 100005, M = 200005;
int n, k, m, z, tot, st[N], bel[N], id[N], lc[M], rc[M], fa[M];
long long a[N];
struct Node {
    long long w, l, r, d;
    int c;
} f[M], t[N << 2];
Node leaf(long long x) { return {0, x, x, x, 1}; }
Node path(long long x) { return {x, x, x, x, 2}; }
Node merge(Node a, Node b, char o, int s = 0) {
    Node z = a;
    if (o == 'R') {
        if (a.c == 1) {
            z.l = z.r = max(a.l, b.l);
        } else if (s == 0) {
            z.l = max(a.l, b.l); z.r = max(a.r, a.w + b.l);
        } else {
            z.l = max(a.l, a.w + b.l); z.r = max(a.r, b.l);
        }
        z.d = max(max(a.d, b.d), @@B1@@);
    } else {
        z.w = a.w + b.w; z.l = max(a.l, a.w + b.l);
        z.r = max(b.r, b.w + a.r);
        z.d = max(max(a.d, b.d), a.r + b.l); z.c = 2;
    }
    return z;
}
int join(int x, int y) {
    int u = ++tot; lc[u] = x; rc[u] = y; fa[x] = fa[y] = u;
    f[u] = merge(f[x], f[y], 'R');
    return u;
}
int build_rake(int l, int r) {
    if (l == r) return l;
    int mid = (l + r) >> 1;
    return join(build_rake(l, mid), build_rake(mid + 1, r));
}
Node star(int x) { return st[x] ? f[st[x]] : leaf(0); }
Node make(int x) {
    Node y = path(a[x]);
    if (st[x]) @@B2@@;
    return y;
}
void build(int x, int l, int r) {
    if (l == r) { t[x] = make(l); return; }
    int mid = (l + r) >> 1;
    build(x << 1, l, mid); build(x << 1 | 1, mid + 1, r);
    t[x] = merge(t[x << 1], t[x << 1 | 1], 'C');
}
void change(int x, int l, int r, int q) {
    if (l == r) { t[x] = make(l); return; }
    int mid = (l + r) >> 1;
    if (q <= mid) change(x << 1, l, mid, q);
    else change(x << 1 | 1, mid + 1, r, q);
    t[x] = merge(t[x << 1], t[x << 1 | 1], 'C');
}
Node query(int x, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) return t[x];
    int mid = (l + r) >> 1;
    if (qr <= mid) return query(x << 1, l, mid, ql, qr);
    if (ql > mid) return query(x << 1 | 1, mid + 1, r, ql, qr);
    Node p = query(x << 1, l, mid, ql, qr);
    Node q = query(x << 1 | 1, mid + 1, r, ql, qr);
    return @@B3@@;
}
void modify(int x, long long y) {
    int u = id[x]; f[u] = leaf(y);
    while (fa[u]) { u = fa[u]; f[u] = merge(f[lc[u]], f[rc[u]], 'R'); }
    if (@@B4@@) change(1, 1, n - 1, bel[x]);
}
int main() {
    cin >> n >> k >> m;
    for (int i = 1; i < n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) {
        int c, l = tot + 1; cin >> c;
        while (c--) {
            long long x; cin >> x; bel[++z] = i; id[z] = ++tot;
            f[tot] = leaf(x);
        }
        if (l <= tot) st[i] = build_rake(l, tot);
    }
    build(1, 1, n - 1);
    while (m--) {
        int o, x, y; cin >> o >> x >> y;
        if (o == 1) { a[x] = y; change(1, 1, n - 1, x); }
        else if (o == 2) modify(x, y);
        else {
            Node ans = @@B5@@;
            if (x < y) ans = merge(ans, star(y), 'R', 1);
            cout << ans.d << endl;
        }
    }
    return 0;
}
