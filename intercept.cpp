#include <cstdio>
#include <dlfcn.h>
#include <cstdarg>
#include <cstdlib>

static int (*original_printf)(const char *, ...) = nullptr;

extern "C" int printf(const char *format, ...)
{
    if (!original_printf)
    {
        original_printf = reinterpret_cast<int (*)(const char *, ...)>(dlsym(RTLD_NEXT, "printf"));
        if (!original_printf)
        {
            std::fprintf(stderr, "Error loading original printf function.\n");
            std::exit(EXIT_FAILURE);
        }
    }

    original_printf("printf has been intercepted!!!: ");

    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);

    return result;
}
