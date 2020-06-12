#!/bin/bash -eu

BASE=$(dirname $0)/..

function test_file()
{
    local json_file=$1
    local smile_file=$2
    echo "***   C   *** Testing $smile_file"

    $BASE/unsmile $BASE/data/smile/$smile_file | diff -du $BASE/data/json/$json_file -
}

for i in 1 2 3 4 5; do
    test_file json-org-sample$i.jsn json-org-sample$i.smile
done

for i in 4k 64k; do
  test_file numbers-int-$i.jsn numbers-int-$i.smile
done

for i in 1 2 3; do
    test_file test$i.jsn test$i.smile
done
