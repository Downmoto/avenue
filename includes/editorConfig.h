#ifndef EDITORCONFIG_H_
#define EDITORCONFIG_H_

#include <termios.h>

struct editorConfig
{
    int cx, cy;
    int rows;
    int cols;
    struct termios orig_termios; // stores orignal terminal flags for restoration
};

#endif