#!/bin/sh

../ftext -c 3 -w 21 -g 6 -l 24 <./input.txt >./provided.txt

if [ $? -gt 0 ]; then
    echo "Test aborted.."
    exit 1
fi

if diff ./expected.txt ./provided.txt; then
    echo "Test passed."
else
    echo "Test failed."
fi
