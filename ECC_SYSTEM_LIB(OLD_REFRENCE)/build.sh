#!/usr/bin/env bash
shopt -s globstar

gcc -I. -IALGORITHMS -IDEBUG -IHASHMAP **/*.c -o test
