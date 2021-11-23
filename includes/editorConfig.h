#ifndef EDITORCONFIG_H_
#define EDITORCONFIG_H_

#include <termios.h>

struct editorConfig
{
    int cx, cy; // cursor position
    int rows; // terminal rows
    int cols; // terminal columns
    struct termios orig_termios; // stores orignal terminal flags for restoration
};

#endif