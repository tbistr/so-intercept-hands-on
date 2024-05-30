#include <stdlib.h>
#include <dlfcn.h>

int main()
{
    void *handle;
    char *error;

    // load libc.so.6 dynamically
    handle = dlopen("libc.so.6", RTLD_LAZY);
    if (!handle)
        exit(1);

    dlerror();

    // search for printf symbol
    auto printf_via_dl = (int (*)(const char *, ...))dlsym(handle, "printf");
    if ((error = dlerror()) != NULL)
        exit(1);

    // call printf
    printf_via_dl("Hello, world!\n");

    // test for func that is not intercepted
    auto getenv_via_dl = (char *(*)(const char *))dlsym(handle, "getenv");
    if (!getenv_via_dl)
        exit(1);

    auto result = getenv_via_dl("SAMPLE_ENV_VAR");
    if (result)
        printf_via_dl("SAMPLE_ENV_VAR: %s\n", result);
    else
        printf_via_dl("SAMPLE_ENV_VAR is not set.\n");

    // close the library
    dlclose(handle);
}
