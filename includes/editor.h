#ifndef EDITOR_H_
#define EDOTOR_H_

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>

#include "INFO.h"
#include "editorKeys.h"
#include "editorConfig.h"
#include "abuf.h"
#include "terminal.h"

#define CTRL_KEY(k) ((k)&0x1f)
#define TAB_STOP 8

// moves cursor based on arrow key press
void editorMoveCursor(int key);

// blocks for key read and returns read key
int editorReadKey();

// processes keys looking for special keypresses i.e ctrl + c to exit()
void editorProcessKeypress();

// TODO: Document function
void editorScroll();

// Draws rows of tildes as well as the welcome screen
void editorDrawRows(struct abuf *ab);

// refreshes terminal screen and writes append buffer to it before freeing it
void editorRefreshScreen();

// TODO: Document function
int editorRowCxToRx(erow *row, int cx);

// TODO: Document function
void editorUpdateRow(erow *row);

// TODO: Document function
void editorAppendRow(char* s, size_t len);

// TODO: Document function
void editorOpen(char* filename);

// initialise editor with windowsize and cursor position
void initEditor();

#endif