#ifndef EDITORCONFIG_H_
#define EDITORCONFIG_H_

#include <termios.h>
#include "erow.h"

struct editorConfig
{
    int cx, cy; // cursor position 
    int rx;
    int rowoff;
    int coloff;
    int rows; // terminal rows
    int cols; // terminal columns
    int numrows;
    erow *row;
    struct termios orig_termios; // stores orignal terminal flags for restoration
};

struct editorConfig E;

#endif