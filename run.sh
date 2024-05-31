#! /bin/bash

export SAMPLE_ENV_VAR="This is a sample environment variable"

echo "=== main.cpp ==="
g++ -o main main.cpp
./main
echo

echo "=== main_via_dlopen.cpp ==="
g++ -o main main_via_dlopen.cpp -ldl
./main
echo

# Generate shared object file for intercepting printf
g++ -shared -fPIC -o intercept.so intercept.cpp -ldl

echo "=== main.cpp (intercepted) ==="
g++ -o main main.cpp -fno-builtin-printf -fno-builtin-fprintf
LD_PRELOAD=./intercept.so ./main
echo

echo "=== main_via_dlopen.cpp (intercepted?) ==="
g++ -o main main_via_dlopen.cpp -fno-builtin-printf -fno-builtin-fprintf -ldl
LD_PRELOAD=./intercept.so ./main
echo

# Generate shared object file for intercepting also dlopen
g++ -shared -fPIC -o intercept_dl.so intercept.cpp intercept_dl.cpp -ldl

echo "=== main_via_dlopen.cpp (intercepted with dlopen) ==="
g++ -o main main_via_dlopen.cpp -fno-builtin-printf -fno-builtin-fprintf -ldl
LD_PRELOAD=./intercept_dl.so ./main
echo

rm main intercept.so intercept_dl.so
