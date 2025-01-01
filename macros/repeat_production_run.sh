#! /bin/bash
N=40
echo "starting $N repetitions"
start=$SECONDS
for ((n=1;n<=$N;n++)); do
    echo "starting $n / $N"
    s1=$SECONDS
    ./doublecap prod.mac > /dev/null 2> "errors.log"
    duration=$(( SECONDS - s1))
    echo "finished: $duration seconds"
    sleep 10
done

duration=$(( SECONDS - start))
echo "$duration seconds total"
