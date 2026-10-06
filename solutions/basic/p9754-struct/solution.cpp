/*
 * P9754 [CSP-S 2023] 结构体 —— 按特殊性质分档的"能拿分"版本
 * ---------------------------------------------------------------------------
 * 本题是纯模拟：维护每种结构体类型的「大小 / 对齐 / 成员偏移表」，
 * 四种操作分别对应：定义类型、定义元素、访问成员、反查地址。
 *
 * 这份实现把四种操作都写了，因此能拿到满分；但它是"**能拿分**"的写法，
 * 不是最省事的写法 —— 真正考场上按下面的特殊性质分档，可以用更短的代码拿到大部分分：
 *
 *   特殊性质 C（操作 1 的成员类型全是基本类型）
 *       ⇒ 结构体不嵌套，不需要递归，偏移表一层就够，代码能砍掉一半。
 *   特殊性质 D（基本类型只有 long）
 *       ⇒ 所有类型的对齐都是 8，alignUp 退化成"直接按 8 取整"。
 *   特殊性质 A（没有操作 4）
 *       ⇒ 完全不写下面的 search()（反查是最容易写错的一段），直接少 20 行。
 *   特殊性质 B（只有一个操作 2）
 *       ⇒ 元素表只会有一条记录，遍历都省了。
 *
 * 编译：g++ -static -O2 -std=c++14 solution.cpp -o solution.exe
 */
#include <bits/stdc++.h>
using namespace std;

struct Member { string name, type; long long off; };
struct Type   { long long sz = 0, al = 1; vector<Member> mem; };
struct Elem   { string name, type; long long st; };

map<string, Type> types;                 // 已定义的结构体类型
map<string, long long> basicSz;          // 基本类型：名 -> 大小（对齐要求 = 大小）
long long curAddr = 0;                   // 下一个元素的分配起点
vector<Elem> elems;                      // 已定义元素

long long alignUp(long long x, long long a) { return (x + a - 1) / a * a; }

/* 取一个类型的大小与对齐要求（基本类型和结构体统一接口） */
void info(const string& t, long long& sz, long long& al) {
    if (basicSz.count(t)) { sz = al = basicSz[t]; }
    else { sz = types[t].sz; al = types[t].al; }
}

/* 操作 4：在类型 tn（起始地址 base）内找地址 target 所属的基本类型成员，
 * 找到就把它相对该类型的路径写进 rel 并返回 true。 */
bool search(const string& tn, long long base, long long target, string& rel) {
    if (basicSz.count(tn))
        return target >= base && target < base + basicSz[tn];
    for (const auto& m : types[tn].mem) {
        size_t save = rel.size();
        rel += "." + m.name;
        if (search(m.type, base + m.off, target, rel)) return true;
        rel.resize(save);
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    basicSz["byte"] = 1; basicSz["short"] = 2;
    basicSz["int"]  = 4; basicSz["long"]  = 8;

    int Q; cin >> Q;
    while (Q-- > 0) {                               // 写成 > 0，Q 异常时也能退出
        int op; cin >> op;

        if (op == 1) {                                  // 定义结构体类型
            string name; int k; cin >> name >> k;
            Type t; long long off = 0, mx = 1;
            for (int i = 0; i < k; ++i) {
                string mt, mn; cin >> mt >> mn;
                long long msz, mal; info(mt, msz, mal);
                off = alignUp(off, mal);                // 先对齐到本成员的边界
                t.mem.push_back({mn, mt, off});
                off += msz;                             // 再占掉它的大小
                mx = max(mx, mal);
            }
            t.al = mx;                                  // 整体对齐 = 成员对齐的最大值
            t.sz = alignUp(off, mx);                    // 整体大小要补齐到对齐的整数倍
            types[name] = t;
            cout << t.sz << " " << t.al << "\n";
        }
        else if (op == 2) {                             // 定义元素
            string mt, mn; cin >> mt >> mn;
            long long msz, mal; info(mt, msz, mal);
            long long st = alignUp(curAddr, mal);       // 起始地址对齐
            elems.push_back({mn, mt, st});
            curAddr = st + msz;
            cout << st << "\n";
        }
        else if (op == 3) {                             // 访问成员 a.b.c
            string path; cin >> path;
            vector<string> part; string s;
            for (char c : path) { if (c == '.') { part.push_back(s); s.clear(); } else s += c; }
            part.push_back(s);

            long long addr = 0; string cur;
            for (const auto& e : elems) if (e.name == part[0]) { cur = e.type; addr = e.st; break; }
            for (size_t i = 1; i < part.size(); ++i)
                for (const auto& m : types[cur].mem)
                    if (m.name == part[i]) { addr += m.off; cur = m.type; break; }
            cout << addr << "\n";
        }
        else {                                          // 操作 4：反查地址
            long long a; cin >> a;
            bool found = false;
            for (const auto& e : elems) {
                string rel;
                if (search(e.type, e.st, a, rel)) { cout << e.name << rel << "\n"; found = true; break; }
            }
            if (!found) cout << "ERR\n";
        }
    }
    return 0;
}
