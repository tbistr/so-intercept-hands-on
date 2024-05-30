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

    flags &= ~RTLD_LOCAL;
    flags |= RTLD_GLOBAL;

    if (std::strcmp(filename, "libc.so.6") == 0)
    {
        static int dummy = 0xdeedbeef;
        Dl_info info;
        if (dladdr(&dummy, &info))
        {
            return original_dlopen(info.dli_fname, flags);
        }
    }
    return original_dlopen(filename, flags);
}
