#ifndef EDITOR_H_
#define EDOTOR_H_

#include <unistd.h>
#include <errno.h>

#include "INFO.h"
#include "editorKeys.h"
#include "editorConfig.h"
#include "abuf.h"
#include "terminal.h"

#define CTRL_KEY(k) ((k)&0x1f)

// moves cursor based on arrow key press
void editorMoveCursor(int key);

// blocks for key read and returns read key
int editorReadKey();

// processes keys looking for special keypresses i.e ctrl + c to exit()
void editorProcessKeypress();

// Draws rows of tildes as well as the welcome screen
void editorDrawRows(struct abuf *ab);

// refreshes terminal screen and writes append buffer to it before freeing it
void editorRefreshScreen();

// initialise editor with windowsize and cursor position
void initEditor();

#endif