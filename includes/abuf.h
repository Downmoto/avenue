#ifndef ABUF_H_
#define ABUF_H_
#define ABUF_INIT {NULL, 0}

#include <string.h>
#include <stdlib.h>

// dynamic append buffer
struct abuf
{
    char *b;
    int len;
};

// constructor for abuf struct
void abAppend(struct abuf *ab, const char *s, int len);

// destructor for abuf struct frees dynamic memory
void abFree(struct abuf *ab);

#endif