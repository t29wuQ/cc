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

try 35 'z = 6;z = 4;a = 3;(z    +  a) *5;'
try 18 'b = 3;(b +    6) * 2;'
echo ok