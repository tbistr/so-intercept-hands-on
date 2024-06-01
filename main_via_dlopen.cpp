#include <iostream>
#include <stdlib.h> // for exit()
#include <dlfcn.h>  // for dlopen(), dlsym()
// we don't need math.h anymore

int main()
{
    char *error;

    // load libm.so dynamically
    auto handle = dlopen("libm.so", RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    dlerror();

    // get pow() address
    auto dl_pow = (double (*)(double, double))dlsym(handle, "pow");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    // get log() address
    auto dl_log = (double (*)(double))dlsym(handle, "log");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    // do same thing as main.cpp
    double result;
    auto x = 8.0;
    auto y = 2.0;
    result = dl_pow(x, y);
    std::cout << "pow(8.0, 2.0) = " << result << std::endl;

    auto e = 2.718282;
    result = dl_log(e);
    std::cout << "log(e) = " << result << std::endl;

    // unload libm.so
    dlclose(handle);
    return 0;
}
