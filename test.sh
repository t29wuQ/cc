#!/bin/sh
try(){
    expected="$1"
    input="$2"

    ./cc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"
    if [ "$actual" != "$expected" ]; then
        echo "$input expected, but got $actual"
        exit 1
    fi
}

try 1 0+1
try 38 32+6
try 5 9-4
try 8 8+5-10+5
echo ok