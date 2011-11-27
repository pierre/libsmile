#!/bin/bash -eu

BASE=$PWD/$(dirname $0)/..

# Recompile to remove debug flags
make clean && make

for i in 1 2 3 4 5; do
    json_file=json-org-sample$i.jsn
    smile_file=json-org-sample$i.smile
    echo "*** Testing $smile_file"
    $BASE/unsmile $BASE/data/smile/$smile_file | diff -du $BASE/data/json/$json_file -
done
