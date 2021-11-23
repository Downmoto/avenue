#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "editorConfig.h"
#include "editorKeys.h"

struct editorConfig E;

// kills program on error and calls exit() to ensure atexit() runs
void die(const char *s);

// returns terminal to canonical and reenables original flags
void disableRawMode();

// enters raw mode by setting correct flags
void enableRawMode();

// blocks for key read and returns read key
int editorReadKey();

// assigns cursor position to arguements return 0 on success
int getCursorPosition(int *rows, int *cols);

// uses sys/ioctl to get window size, with fallback to manual get
int getWindowsize(int *rows, int *cols);

#endif