#!/bin/bash
total=20;
i=0
while(( $i<= $total ))
do
    if [ -e "./$i-in.ss" ]; then
       echo ------ run test $i started ---------
       ../bin/test-csk < ./$i-in.ss | diff - ./$i-out.ss
       echo ------ run test $i finished ---------
    fi
    let i=$((i+1))
done
