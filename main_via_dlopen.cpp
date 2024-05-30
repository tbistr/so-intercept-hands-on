#include <stdlib.h>
#include <dlfcn.h>

int main()
{
    void *handle;
    int (*printf_via_dlopen)(const char *format, ...);
    char *error;

    // load libc.so.6 dynamically
    handle = dlopen("libc.so.6", RTLD_LAZY);
    if (!handle)
        exit(1);

    dlerror();

    // search for printf symbol
    *(void **)(&printf_via_dlopen) = dlsym(handle, "printf");
    if ((error = dlerror()) != NULL)
        exit(1);

    // call printf
    printf_via_dlopen("Hello, world!\n");

    // close the library
    dlclose(handle);
}
