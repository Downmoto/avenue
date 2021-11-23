// includes

#include <ctype.h>
#include <string.h>

#include "editorConfig.h"
#include "terminal.h"
#include "editorKeys.h"

// defines

#define AVENUE_VERSION "0.0.1"

#define CTRL_KEY(k) ((k) & 0x1f)
#define ABUF_INIT {NULL, 0}



// data

// dynamic append buffer
struct abuf
{
    char *b;
    int len;
};

// global struct containing editor state information

// constructor for abuf struct
void abAppend(struct abuf* ab, const char* s, int len)
{
    char* new = realloc(ab->b, ab->len + len);
    
    if (new == NULL) return;
    memcpy(&new[ab->len], s, len);
    ab->b = new;
    ab->len += len;
}

// destructor for abuf struct frees dynamic memory
void abFree(struct abuf *ab)
{
    free(ab->b);
}

// input

void editorMoveCursor(int key)
{
    switch (key)
    {
    case ARROW_LEFT:
        if (E.cx != 0)
        {
            E.cx--;
        }
        break;
    case ARROW_RIGHT:
        if (E.cx != E.cols - 1)
        {
            E.cx++;
        }
        break;
    case ARROW_UP:
        if (E.cy != 0)
        {
            E.cy--;
        }
        break;
    case ARROW_DOWN:
        if (E.cy != E.rows - 1)
        {
            E.cy++;
        }
        break;
    }
}

// processes keys looking for special keypresses i.e ctrl + c to exit()
void editorProcessKeypress()
{
    int c = editorReadKey();

    switch (c)
    {
    case CTRL_KEY('c'):
        write(STDOUT_FILENO, "\x1b[2J", 4); // clears entire screen
        write(STDOUT_FILENO, "\x1b[H", 3); // sends cursor home
        exit(0);
        break;

    case HOME_KEY:
        E.cx = 0;
        break;
    case END_KEY:
        E.cx = E.cols - 1;
        break;

    case PAGE_UP:
    case PAGE_DOWN:
        {
            int times = E.rows;
            while (times--)
                editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);            
        }
        break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
        editorMoveCursor(c);
        break;
    }
}

// output

// Draws rows of tildes as well as the welcome screen
void editorDrawRows(struct abuf* ab)
{
    int y;
    for (y = 0; y < E.rows; y++)
    {
        if (y == E.rows / 3)
        {
            char welcome[80];
            int welcomelen = snprintf(welcome, sizeof(welcome),
                "Avenue editor -- version %s", AVENUE_VERSION);
            
            if (welcomelen > E.cols)
                welcomelen = E.cols;

            int padding = (E.cols - welcomelen) / 2;
            if (padding)
            {
                abAppend(ab, "~", 1);
                padding--;
            }
            while (padding--) 
                abAppend(ab, " ", 1);

            abAppend(ab, welcome, welcomelen);
        }
        else
        {
            abAppend(ab, "~", 1);
        }

        abAppend(ab, "\x1b[K", 3);
        if (y < E.rows - 1)
            abAppend(ab, "\r\n", 2);
    }
}

// refreshes terminal screen and writes append buffer to it before freeing it
void editorRefreshScreen()
{
    struct abuf ab = ABUF_INIT;

    abAppend(&ab, "\x1b[?25l", 6);
    abAppend(&ab, "\x1b[H", 3);

    editorDrawRows(&ab);

    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
    abAppend(&ab, buf, strlen(buf));

    abAppend(&ab, "\x1b[?25h", 6);

    write(STDOUT_FILENO, ab.b, ab.len);
    abFree(&ab);
}

// init

// initializes global state struct with rows and cols
void initEditor()
{
    E.cx = 0;
    E.cy = 0;

    if (getWindowsize(&E.rows, &E.cols) == -1) 
        die("getWindowSize");
}

int main()
{
    enableRawMode();
    initEditor();

    while (1)
    {
        editorRefreshScreen();
        editorProcessKeypress();
    }

    return 0;
}
