#include <dlfcn.h>
#include <iostream>

static double (*original_pow)(double, double) = nullptr;

extern "C" double pow(double x, double y)
{
    // Load original pow() if not loaded yet
    if (!original_pow)
    {
        original_pow = reinterpret_cast<double (*)(double, double)>(dlsym(RTLD_NEXT, "pow"));
        if (!original_pow)
        {
            std::cout << "Error loading original pow function." << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    // Do some interception
    x *= 2;
    y *= 2;

    std::cout << "\033[31m";
    std::cout << "Indercepted! Args are doubled: (" << x << ", " << y << ")" << std::endl;
    std::cout << "\033[0m";

    // Call original pow()
    return original_pow(x, y);
}
