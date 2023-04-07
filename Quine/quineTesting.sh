#!/bin/bash

tcc generateQuineString.c
./generateQuineString.exe < quinePrint.c > isThisAQuine.c

cat quinePrint.c >> isThisAQuine.c

tcc isThisAQuine.c
./isThisAQuine.exe > output.txt

diff output.txt isThisAQuine.c > diff.txt
rm ./isThisAQuine.*
rm ./generateQuineString.exe