#include <dlfcn.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

static void *(*original_dlopen)(const char *filename, int flags) = nullptr;

extern "C" void *dlopen(const char *filename, int flags)
{
    if (!original_dlopen)
    {
        original_dlopen = reinterpret_cast<void *(*)(const char *, int)>(dlsym(RTLD_NEXT, "dlopen"));
        if (!original_dlopen)
        {
            std::fprintf(stderr, "Error loading original dlopen function.\n");
            std::exit(EXIT_FAILURE);
        }
    }

    // We want to expose symbols of libc.so.6 to other libraries
    flags &= ~RTLD_LOCAL;
    flags |= RTLD_GLOBAL;

    if (std::strcmp(filename, "libc.so.6") == 0)
    {
        // Assume that no other library has symbol such as 0xdeedbeef
        static int dummy = 0xdeedbeef;
        Dl_info info;
        if (dladdr(&dummy, &info))
        {
            // Load ownself instead of libc.so.6
            // This program have loaded libc.so.6 implicitly.
            // Thus we can expose symbols of original libc.so.6
            return original_dlopen(info.dli_fname, flags);
        }
    }
    return original_dlopen(filename, flags);
}
