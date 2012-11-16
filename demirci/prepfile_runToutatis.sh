#!/bin/bash
TOUTATISPATH=/Applications/Toutatis.app/Contents/MacOS

cat ex_start.txt  > myex.inp
cat dmr.out  >> myex.inp
cat ex_finish.txt >> myex.inp


$TOUTATISPATH/Toutatis -i myex.inp

