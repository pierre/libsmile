#!/bin/bash -eu

BASE=$(dirname $0)/..

for file in test1 test2; do
    $BASE/src/smile $BASE/../data/smile/$file.smile | diff -du $BASE/../data/json/$file.jsn -
done
