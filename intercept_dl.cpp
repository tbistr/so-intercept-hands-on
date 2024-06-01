#include <dlfcn.h>
#include <string>
#include <iostream>

static void *(*original_dlopen)(const char *filename, int flags) = nullptr;

extern "C" void *dlopen(const char *filename, int flags)
{
    if (!original_dlopen)
    {
        original_dlopen = reinterpret_cast<void *(*)(const char *, int)>(dlsym(RTLD_NEXT, "dlopen"));
        if (!original_dlopen)
        {
            std::cout << "Error loading original dlopen function." << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    if (std::string(filename) == "libm.so")
    {
        // Set dummy variable, it has static address.
        static int dummy = 0xdeedbeef;
        Dl_info info;
        // Find .so info which contains dummy variable address (=ownself).
        if (dladdr(&dummy, &info))
        {
            // Load ownself instead of libm.so
            // This program have loaded libm.so implicitly (see ldd ./intercept_dl.so).
            // Thus we can expose symbols of original libm.so.
            // If you want to hook library which is not loaded yet, you should load it explicitly with RTLD_GLOBAL.
            // ex. original_dlopen("libm.so", RTLD_LAZY | RTLD_GLOBAL);
            return original_dlopen(info.dli_fname, flags);
        }
    }
    return original_dlopen(filename, flags);
}
