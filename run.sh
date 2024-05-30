#! /bin/bash

echo "=== main.cpp ==="
g++ -o main main.cpp -ldl
./main
echo
echo "=== main_via_dlopen.cpp ==="
g++ -o main main_via_dlopen.cpp -ldl
./main

rm main
