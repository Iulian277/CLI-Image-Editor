#include "../headers/commons.h"

int scc(int code)
{
    if (code < 0)
    {
        fprintf(stderr, "ERROR: Return code %d\n", code);
        exit(1);
    }

    return code;
}

void *scp(void *ptr)
{
    if (ptr == NULL)
    {
        fprintf(stderr, "ERROR: Null pointer exception\n");
        exit(1);
    }

    return ptr;
}

FILE *open_file(const char *file_path, const char *mode)
{
    FILE *file = scp(fopen(file_path, mode));
    return file;
}