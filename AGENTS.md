# Agent 工作规范

本文件约束 Agent 在此仓库内的所有操作。每次会话开始写代码前必须先读本文件。

---

## 1. 项目定位

为**讲课**服务的 OI 题解知识库。每道题的输出对象是"正在听讲的初三到高中学生"，不是评测机。  
写作质量标准见 [`notes/CONVENTIONS.md`](notes/CONVENTIONS.md)。

---

## 2. 目录规范

```
solutions/<主题>/<题号-slug>/   # 一题一目录，见 CONVENTIONS.md
notes/                           # 跨题讲义、比赛规律总结、教案
templates/                       # 写作模板，只读
scripts/                         # 工具脚本
```

新增题目：  
```bash
bash scripts/new_problem.sh <P题号> <主题路径> <中文名>
# 例：bash scripts/new_problem.sh P1048 dp/knapsack 采药
```

---

## 3. 每道题的工作流

按顺序完成以下步骤，不得跳过验证直接写答案：

### 3.1 读题与代码验证

1. 将用户提供的题面和代码写入 `programs/j1.cpp`（或对应编号）。
2. **本地编译并运行**，记录确定输出：
   ```bash
   g++ -static -O2 -std=c++14 programs/j1.cpp -o programs/j1.exe
   ./programs/j1.exe
   ```
   本机有两套 MinGW 路径冲突，**必须加 `-static`**，否则段错误。
3. 输出结果与手推不符时，以程序实际输出为准，修改推导而不是修改程序。

### 3.2 写题解 README.md

结构固定（来自 `templates/solution-template.md`）：

```
题目大意 → 暴力想法 → 关键观察 → 算法设计 → 正确性论证 → 复杂度
→ 可视化讲解 → 讲解代码 → 易错点 → 常见变式 → 一句话提炼
```

- **"一句话提炼"** 放在末尾，是讲课时板书用的，必须可独立引用。
- 对用户原始代码的问题要**直接指出**，不能静默替换。

### 3.3 写可视化

单文件 `visualization.html`，约束：
- 内联 CSS/JS，无外部 CDN，浏览器直接打开。
- 控件：上一步 / 下一步 / 自动播放 / 跳到末尾。
- 有代码行高亮 + 变量状态表（或数组条形图）。
- 深色主题，沿用现有 HTML 配色（`--bg:#0f1220` 等）。

### 3.4 更新索引

新增题目后，在根目录 `README.md` 的题目列表表格追加一行。

### 3.5 记录版本

见第 4 节。

---

## 4. 版本管理规范

### 4.1 Git 提交

每次完成一个逻辑单元（新增一道题、修改一份讲义、修复一个可视化）后提交：

```bash
git add <files>
git commit -m "<type>(<scope>): <subject>"
```

type 取值：`feat`（新题）、`fix`（纠错）、`docs`（讲义补充）、`style`（可视化调整）、`chore`（脚本/规范）。

**不要 push**，除非用户明确说"推上去"。

### 4.2 CHANGELOG.md

仓库根目录维护 `CHANGELOG.md`，格式：

```markdown
## YYYY-MM-DD

- **新增** P1048 采药（dp/knapsack）：完整题解 + 背包分步可视化
- **修复** P14357 visualization.html 桶高度在 n>50 时溢出
- **讲义** notes/luogu-2026-j1/、notes/luogu-2026-s1/：2026 第一轮卷子原文讲评（题面见 paper.md）
- **讲义** notes/mock-cspjs-round1/：自编仿真题（非真题），含 J/S 各 3 篇阅读程序可视化
```

每次 commit 前更新 CHANGELOG.md，把 CHANGELOG.md 和改动文件一起加入同一次提交。

### 4.3 metadata.yml 状态字段

`status` 字段说明真实情况，不得虚报：

- `AC`：已通过洛谷/NOI 评测（需要用户提供截图或确认）
- `未本地评测（原因）`：没有网络评测环境时的默认值
- `对拍验证`：与暴力版随机对拍 N 组一致，写明 N

---

## 5. 洛谷题目抓取

洛谷题目页需要登录，直接 WebFetch 拿不到题面。  
用户可手动保存 HTML 后运行：

```bash
node scripts/parse_luogu.js <保存的html路径>
```

脚本从 SSR JSON 中提取题面字段，输出到 stdout。

---

## 5.5 真题卷（整卷 PDF）的题面流水线

`notes/ccf-zhenti-2026-j1/`（CCF 官方卷）、`notes/luogu-2026-j1/`、`notes/luogu-2026-s1/`（洛谷命制）是**整卷讲评**，
题面来自用户提供的 PDF / 照片。
规则：**题目文字只允许出现在 `problem.txt` 里，别的地方一律由脚本机械生成**，
这样"抄错题目"这种事不可能发生。卷面缺页就在 `problem.txt` 里写一行"原始资料缺页"，**绝不补写**。

| 文件 | 谁写的 | 作用 |
|---|---|---|
| `problem.txt` | 人手抄自 PDF | 唯一题面来源，保留卷面行号（空行也带行号） |
| `paper.md` | `scripts/paper_from_text.js` | 整卷好读版：去页眉页脚和广告、程序包成代码块、卷面答案在最后 |
| `README.md` 里的 `> **卷面原文**` / `> **卷面完整程序**` 引用块 | `scripts/inject_paper_text.js` | 逐题、逐篇把题面贴进讲评对应小节 |
| `README.md` 的其余正文 | 人手写 | 讲评本体，脚本不碰 |

```bash
# 重新生成整卷题面（J 卷行号在行首，S 卷在行尾）
node scripts/paper_from_text.js notes/luogu-2026-j1 --num-at start --pdf "SCP2026 J1 全卷（附答案）.pdf" --paper SCP-J1
node scripts/paper_from_text.js notes/luogu-2026-s1 --num-at end   --pdf "SCP2026 S1 全卷（附答案）.pdf" --paper SCP-S1
# CCF 官方真卷（--pdf / --paper 必须照抄，漏掉会把 paper.md 的标题退化成目录名）
node scripts/paper_from_text.js notes/ccf-zhenti-2026-j1 --num-at start --pdf "CCF CSP-J 2026 第一轮（照片整理版）" --paper CSP-J1
# 往 README 贴卷面原文（可反复执行，已贴过的小节会自动跳过）
node scripts/inject_paper_text.js notes/luogu-2026-j1 --blanks 33-37,38-42
node scripts/inject_paper_text.js notes/luogu-2026-s1 --blanks 34-38,39-43
node scripts/inject_paper_text.js notes/ccf-zhenti-2026-j1 --blanks 34-38,39-43
```

注意：`> **卷面原文**` 引用块是生成内容，**不要手改**；要改题面就改 `problem.txt` 再重跑。

**整卷目录的前缀有三级含义，新建目录时按这个选，不要笼统叫"真题"：**

| 前缀 | 含义 | 现有目录 |
|---|---|---|
| `ccf-` | **CCF 官方发布的真卷**（如 2026-09-19 认证的 CSP-J1） | `notes/ccf-zhenti-2026-j1/` |
| `luogu-` | **洛谷命制**的卷子（页眉写 SCP-J1/S1，卷面自己声明"由洛谷网校学术组命制"） | `notes/luogu-2026-j1/`、`notes/luogu-2026-s1/` |
| `mock-` | **我自己编**的仿真题 | `notes/mock-cspjs-round1/`、`notes/mock-cspj-round2/` |

自编仿真题在 `notes/mock-cspjs-round1/`、`notes/mock-cspj-round2/`，不是任何一年的真题，别混用。

---

## 6. 推送

远端目前指向镜像 `gh-proxy.com`（直连 github.com 被重置）。  
如需 push：

```bash
git remote set-url origin https://github.com/fenghuangyuan1412/oi-solutions.git
git push origin main
```

推送前必须先跑 `git status` 和 `git log --oneline -5`，确认不会覆盖用户本地未 push 的改动。  
**任何情况下不得 `--force`**，除非用户明确授权。
