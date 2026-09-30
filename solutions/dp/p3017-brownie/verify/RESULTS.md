# P3017 验证记录（全部为本机实测，未做洛谷提交）

环境：Windows + Git Bash，`g++ (MinGW-Builds) 13.1.0`，Node v24.21.0，Python 3.13.2。
所有 `.exe` / `*.txt` 临时文件都放在 `$TMPDIR/p3017`，跑完删除，仓库里只留源码。

> 本目录下 `brute.cpp` / `dp_check.cpp` / `brute_global.cpp` / `gen.py` / `stress.js` **仅验证用，非讲解代码**；
> 讲解代码只有一处：上一级目录的 `solution.cpp`（C++14）。

---

## 0. 编译命令（逐字照抄可复现）

```bash
mkdir -p /tmp/p3017 && cd /tmp/p3017
P=/e/ai/suanfa_study/oi-solutions/solutions/dp/p3017-brownie

g++ -static -O2 -std=c++14 $P/solution.cpp          -o sol.exe
g++ -static -O2 -std=c++14 $P/verify/brute.cpp      -o brute.exe
g++ -static -O2 -std=c++14 $P/verify/dp_check.cpp   -o dp.exe
g++ -static -O2 -std=c++14 $P/verify/brute_global.cpp -o bglobal.exe
```

四个都编译通过，0 错误 0 警告。`-static` 必须加（本机两套 MinGW 冲突，非静态直接段错误）。

## 1. 题面示例（唯一可用的样例）

`in1.txt`（内容与洛谷页面 `题目描述` 里的那份切割图完全一致）：

```
5 4 4 2
1 2 2 1
3 1 1 1
2 0 1 3
1 1 1 1
1 1 1 1
```

```bash
$ ./sol.exe < in1.txt
3
$ ./brute.exe < in1.txt
3
$ ./dp.exe < in1.txt
3
```

题面自己给出的答案是 3，三个实现都输出 3。**注意**：这份快照没有登录，页面上的
`输入样例/输出样例` 区块没渲染出来（但 `samples` 字段在页面的 SSR JSON 里，内容与题面示例同一组，
输入 `5 4 4 2 / ...`、输出 `3`）。所以它既是题面示例也是唯一样例，**不称之为"洛谷评测样例"**。

## 2. 随机对拍（正解 vs 独立暴力）

`stress.js`（仅验证用）：内置 JS 生成器出小规模数据（R,C≤6，豆数 0~9，两成数据用 0/1 逼并列），
同一份数据分别喂 `sol.exe` 与 `brute.exe`，逐字节比 trim 后的 stdout，不一致就打印该组并退出码 1。

```bash
$ node $P/verify/stress.js ./sol.exe ./brute.exe $P/verify/gen.py 5000
共 5000 组，不一致 0 组
real    0m50.516s

$ node $P/verify/stress.js ./sol.exe ./brute.exe $P/verify/gen.py 400 5 8      # R,C ∈ [5,8]
共 400 组，不一致 0 组

$ node $P/verify/stress.js ./sol.exe ./brute.exe $P/verify/gen.py 300 1 --use-gen  # 改用 gen.py 出数据
共 300 组，不一致 0 组
```

**合计 5700 组，0 不一致。**

另外两路交叉验证（暴力口径不变，换被测实现）：

```bash
$ node $P/verify/stress.js ./dp.exe ./brute.exe $P/verify/gen.py 800
共 800 组，不一致 0 组            # DP 版 check（O(R^2*C)/次）也与暴力全对
```

正解 vs DP 版（中等规模，暴力已跑不动）：

```bash
$ for s in $(seq 1 200); do
    python $P/verify/gen.py $s mid > m.txt          # R,C<=30, 豆数 0..4000
    a=$(./sol.exe < m.txt); b=$(./dp.exe < m.txt)
    [ "$a" = "$b" ] && ok=$((ok+1)) || bad=$((bad+1))
  done
mid 对拍: ok=200 bad=0
```

## 3. 定向边界用例（sol / brute / dp 三方一致）

| 用例 | 输入 | sol | brute | dp |
|---|---|---|---|---|
| 1×1，值为 0 | `1 1 1 1` / `0` | 0 | 0 | 0 |
| 1×1，值 4000 | `1 1 1 1` / `4000` | 4000 | 4000 | 4000 |
| 全零 3×3 | `3 3 2 2` / 9 个 0 | 0 | 0 | 0 |
| A=R 且 B=C（每格一块） | `3 3 3 3` + 3×3 | 1 | 1 | 1 |
| A=B=1（整块不切） | `2 2 1 1` / 1 2 / 3 4 | 10 | 10 | 10 |
| 只有 1 行 | `1 5 1 3` / `2 2 2 2 2` | 2 | 2 | 2 |
| 只有 1 列 | `5 1 3 1` / 3 1 4 1 5 | 4 | 4 | 4 |
| 题面示例 | 见 §1 | 3 | 3 | 3 |

## 4. 暴力为什么炸（实测计时）

`brute.cpp` 是"枚举 A-1 个水平切点 → 每条带独立枚举 B-1 个垂直切点 → 取 A*B 块最小值 → 再对水平划分取最大值"，
配置数 $=\binom{R-1}{A-1}\binom{C-1}{B-1}^{A}$（Python `math.comb` 算的精确值取科学计数）：

| R×C，A，B | 配置数 | brute.exe 实测 | sol.exe 实测 | 两者答案 |
|---|---|---|---|---|
| 5×4, A=4, B=2 | 3.24e2 | 0.033s | 0.033s | 3 = 3 |
| 8×8, A=4, B=4 | 5.25e7 | 0.055s | — | 10 |
| 12×12, A=6, B=6 | 4.49e18 | 196ms | 0.037s | 11 = 11 |
| 14×14, A=7, B=7 | 7.52e25 | 1739ms | 0.052s | 10 = 10 |
| 16×16, A=8, B=8 | 1.89e34 | **30385ms** | 0.032s | 11 = 11 |
| 20×20, A=10, B=10 | 4.18e54 | 未跑（预估 >1e6 s） | — | — |
| 500×500, A=250, B=250 | 10^37340 | 不可能 | 0.07s | — |

16×16 只是把枚举量放大到 2^15 水平掩码 × 2^15 垂直掩码/带，就已经 30 秒；
把 R、C 推到 500 时，方案数连写都写不下。这就是必须先做"判定化"（二分答案）的理由。

## 5. check 里"选带"用 DP 还是用贪心：实测差别（本节的重点）

| 版本 | 单次 check | 文件 |
|---|---|---|
| `dp.exe`（README §3 的 $f[i]$ DP，枚举所有 $(k,i]$ 带） | $O(R^2 C)$ | verify/dp_check.cpp |
| `sol.exe`（单调性 → 每条带取最短可行长度） | $O(RC)$ | solution.cpp |

同一批 $R=C=500$ 数据（`/usr/bin/time` 报 real，含 1.2MB 读入）：

| 数据（首行 R C A B） | dp.exe | sol.exe | 答案（两者相同） |
|---|---|---|---|
| `500 500 69 292` | 0.716s | 0.068s | 15581 |
| `500 500 490 442` | 0.469s | 0.084s | 380 |
| `500 500 122 304` | 0.587s | 0.086s | 8604 |
| `500 500 500 1` | **1.264s** | 0.069s | 908404 |
| `500 500 500 500` | 0.477s | 0.053s | 0 |
| `500 500 250 250` | 0.579s | 0.062s | 5187 |
| `500 500 1 1` | 0.100s | 0.078s | 499836027 |
| 全 4000，`500 500 250 250` | 0.581s | 0.070s | 16000 |
| 全 4000，`500 500 1 1` | 0.102s | 0.087s | **1000000000** |

结论（写成讲课口径）：

- `dp.exe` 是**理论上 $O(R^2C\log V)$（约 $1.9\times10^9$ 次列访问）**的做法，本机最慢 1.26s，
  洛谷 1s 时限（页面 `limits.time = 1000ms`）下**贴着线甚至超线**，属于"能过一部分点、不稳"。
- `sol.exe` 靠"合格性对扩行单调"把 DP 压成每带取最短终点，$O(RC\log V)$（约 $8\times10^6$），
  本机全表 **≤0.09s**，其中大部分还是 1.2MB 的读入时间。
- 最后一行给出 $500\times500\times4000 = 10^9$ 的真实最大总和：`int` 上限 $2.1\times10^9$，
  只剩 2 倍余量，`A*B*mid` 之类的写法一乘就炸 → check 里一律 `long long`。

## 6. 最大规模压力

```bash
$ python $P/verify/gen.py 1 big > big1.txt      # 500 500 69 292，1180966 字节
$ head -1 big1.txt && ./sol.exe < big1.txt      # 500 500 69 292 -> 15581
```

`sol.exe` 在 $R=C=500$、豆数 0~4000 的三组随机大数据 + 两组全 4000 数据上都是 **0.05~0.09s**，
与 DP 版答案逐位相同。

## 7. 未做到 / 需要说明的事

- **未在洛谷提交**，没有 AC 截图，`metadata.yml` 的 `status` 据此写"对拍验证（5700 组）+ 未本地评测洛谷"。
- 官方 `输入样例/输出样例` 区块因未登录没有渲染，只有题面内嵌的那组示例可用（§1），
  README 与 metadata 里一律标注 **题面示例（非洛谷评测样例）**。
- `verify/brute_global.cpp` 是"把带内独立竖切误读成全局竖切"的反例暴力：
  题面示例上它输出 **2**（正解 3）；300 组随机小数据里有 **17 组**与正解不同。
  这个文件只为 §7 的易错点提供可复现数字，不参与对拍判定。

## 8. 变式：二分 + 二维前缀和 + 贪心选带（`sol_2dpre_greedy.cpp`）

用户提出"想用二分答案 + 二维前缀和"，并贴出洛谷常见标程作参考。实测确认这条思路**正确且不超时**，
超时风险只来自把二维前缀和配 $O(R^2)$ 的选带 DP（`dp_check.cpp`），与二维前缀和本身无关。

```bash
g++ -static -O2 -std=c++14 verify/sol_2dpre_greedy.cpp -o g2d.exe
g++ -static -O2 -std=c++14 solution.cpp -o sol.exe
g++ -static -O2 -std=c++14 verify/brute.cpp -o brute.exe

# 题面示例三方一致
$ ./g2d.exe < in1.txt   # 3
$ ./sol.exe < in1.txt   # 3
$ ./brute.exe < in1.txt # 3

# 变式 vs 独立暴力
$ node verify/stress.js ./g2d.exe ./brute.exe verify/gen.py 3000
共 3000 组，不一致 0 组
```

$R=C=500$ 计时（`/usr/bin/time`，本机 g++ 13.1 `-static -O2`）：

| 数据 | `sol_2dpre_greedy` | 洛谷标程口径（上界取 total） | `solution.cpp` | DP 选带版 |
| --- | --- | --- | --- | --- |
| `500 500 250 250` 随机 | 输出 5203 | 输出 5203 | 输出 5203 | — |
| `500 500 500 1` 最坏 | **0.138 s** | **0.354 s** | 0.069 s | 1.264 s（超时） |

三方（本变式 / `solution.cpp` / 暴力）在最坏组输出逐位相同（`924189`）。
结论写进 README「变式：二维前缀和 + 贪心选带」一节。临时 exe 与数据在 `$TMPDIR`，跑完删除，仓库只留源码。
