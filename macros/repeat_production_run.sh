#! /bin/bash
N=10
for ((n=1;n<=$N;n++)); do
    ./doublecap prod.mac > "prod_output_$n.txt"
    echo $n / $N
done
