#!/usr/bin/env bash
# 新建一道题的目录骨架：./scripts/new_problem.sh P1048 dp/knapsack 采药
set -euo pipefail

id="${1:?用法: new_problem.sh <题号> <主题路径> <题名>}"
topic="${2:?缺少主题，如 dp/knapsack}"
title="${3:?缺少题名}"

slug=$(printf '%s' "$id" | tr 'A-Z' 'a-z')
dir="solutions/${topic%%/*}/${slug}"

[ -d "$dir" ] && { echo "已存在: $dir"; exit 1; }
mkdir -p "$dir"

cp templates/solution-template.md "$dir/README.md"
cp templates/metadata-template.yml "$dir/metadata.yml"

cat > "$dir/solution.cpp" <<'EOF'
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}
EOF

cat >> "$dir/metadata.yml" <<EOF

# scaffold 生成于 $(date -I)
EOF

# 用题名/题号回填占位符
sed -i "s|P{编号} {题目名称}|${id} ${title}|" "$dir/README.md"
sed -i "s|Pxxxx|${id}|g" "$dir/README.md" "$dir/metadata.yml"
sed -i "s|P0000|${id}|" "$dir/metadata.yml"
sed -i "s|title: 题目名称|title: ${title}|" "$dir/metadata.yml"
sed -i "s|topics: \[dp/knapsack\]|topics: [${topic}]|" "$dir/metadata.yml"

echo "已创建: $dir"
