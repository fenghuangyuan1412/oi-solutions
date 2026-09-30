#!/bin/bash
# 题 27：随机生成 n 位 + n 位（无前导零）且和 < 10^n 的输入，看输出长度和首字符
srand=$RANDOM
bad=0; tot=0
for i in $(seq 1 300); do
  n=$(( (i % 8) + 1 ))
  a=$(( (RANDOM % (9 * 10**(n-1))) + 10**(n-1) ))
  b=$(( (RANDOM % (9 * 10**(n-1))) + 10**(n-1) ))
  s=$((a + b))
  lim=$((10**n))
  if [ $s -ge $lim ]; then continue; fi
  out=$(echo "$a $b" | ./ip2.exe)
  tot=$((tot+1))
  len=${#out}
  first=${out:0:1}
  if [ "$len" != "$((n+1))" ] || [ "$first" != "0" ]; then
    echo "反例 n=$n 输入=$a $b 输出=$out 长度=$len 首字符=$first"; bad=$((bad+1))
  fi
done
echo "题27  符合条件的随机组数 $tot，长度=n+1 且首字符='0' 全部成立？反例 $bad 个"
