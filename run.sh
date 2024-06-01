#! /bin/bash

export SAMPLE_ENV_VAR="This is a sample environment variable"

g++ -O0 -o main main.cpp -lm
g++ -O0 -o main_via_dl main_via_dlopen.cpp -ldl

echo "=== Running without interception ==="
echo "> main.cpp"
./main
echo
echo "> main_via_dlopen.cpp"
./main_via_dl
echo "===================================="
echo

# Generate shared object file for intercepting printf
g++ -O0 -shared -fPIC -o intercept.so intercept.cpp -ldl

echo "=== Running with native interception ==="
echo "> main.cpp (intercepted)"
LD_PRELOAD=./intercept.so ./main
echo
echo "> main_via_dlopen.cpp (intercepted?)"
LD_PRELOAD=./intercept.so ./main_via_dl
echo "========================================"
echo

# Generate shared object file for intercepting also dlopen
g++ -O0 -shared -fPIC -o intercept_dl.so intercept.cpp intercept_dl.cpp -ldl

echo "=== Running with dlopen interception ==="
echo "> main.cpp (intercepted with dlopen)"
LD_PRELOAD=./intercept_dl.so ./main
echo
echo "> main_via_dlopen.cpp (intercepted with dlopen)"
LD_PRELOAD=./intercept_dl.so ./main_via_dl
echo "========================================="

rm main main_via_dl intercept.so intercept_dl.so
