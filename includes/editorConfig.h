#ifndef EDITORCONFIG_H_
#define EDITORCONFIG_H_

#include <termios.h>
#include "erow.h"

struct editorConfig
{
    int cx, cy; // cursor position 
    int rx;
    int rowoff; // row offset
    int coloff; // column offset
    int rows; // terminal rows
    int cols; // terminal columns
    int numrows; // number of rows
    erow *row; // array of erows
    struct termios orig_termios; // stores orignal terminal flags for restoration
};

struct editorConfig E;

#endif