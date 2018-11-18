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

try 45 'abcd = 6;efgh = 3;(abcd    +  efgh) *5;'
try 18 'b2  = 3;(b2 +    6) * 2;'
try 25 'len = 0;len = 2 + 3;len * 5'
echo ok