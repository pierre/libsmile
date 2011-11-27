#!/bin/bash -eu

BASE=$PWD/$(dirname $0)/..

# Recompile to remove debug flags
make clean && make

function test_file()
{
    local json_file=$1
    local smile_file=$2
    echo "*** Testing $smile_file"
    $BASE/unsmile $BASE/data/smile/$smile_file | diff -du $BASE/data/json/$json_file -
}

for i in 1 2 3 4 5; do
    test_file json-org-sample$i.jsn json-org-sample$i.smile
done
for i in 1 2; do
    test_file test$i.jsn test$i.smile
done
