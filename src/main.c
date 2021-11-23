// includes

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// defines

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
struct editorConfig
{
    int rows;
    int cols;
    struct termios orig_termios; // stores orignal terminal flags for restoration
};

struct editorConfig E;
// terminal

// kills program on error and calls exit() to ensure atexit() runs
void die(const char* s)
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(s);
    exit(1);
}

// returns terminal to canonical and reenables original flags
void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
        die("tcsetattr");
}

// enters raw mode by setting correct flags
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)
        die("tcgetattr");
        
    atexit(disableRawMode);

    // used for altering terminal flags while keeping original flag info safe
    struct termios raw = E.orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8); // character size 8 bits per byte
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    // control characters used by read()
    raw.c_cc[VMIN] = 0; // minimum number of bytes before read() can return
    raw.c_cc[VTIME] = 1; // max amount of time read() waits before returns 

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr|enableRawMode()");
}

// blocks for key read and returns read key
char editorReadKey()
{
    int nread;
    char c;

    while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
        if (nread == -1 && errno != EAGAIN)
            die("read");

    return c;
}

// assigns cursor position to arguements return 0 on success
int getCursorPosition(int *rows, int *cols)
{
    char buf[32];
    unsigned int i = 0;

    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
        return -1;

    while (i < sizeof(buf) -1)
    {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) 
            break;
        if (buf[i] == 'R')
            break;
        i++;
    }
    buf[i] = '\0';

    if (buf[0] != '\x1b' || buf[1] != '[')
        return -1;
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
        return -1;

    return 0;
}

// uses sys/ioctl to get window size, with fallback to manual get
int getWindowsize(int* rows, int* cols)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
            return -1;
        
        return getCursorPosition(rows, cols);
    }
    else
    {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

// input

// processes keys looking for special keypresses i.e ctrl + c to exit()
void editorProcessKeypress()
{
    char c = editorReadKey();

    switch (c)
    {
    case CTRL_KEY('c'):
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0);
        break;

    default:
        printf("%c\r\n", c);
        break;
    }
}

// output

// Draws rows of tildes
void editorDrawRows()
{
    int y;
    for (y = 0; y < E.rows; y++)
    {
        write(STDOUT_FILENO, "~", 1);

        if (y < E.rows - 1)
            write(STDOUT_FILENO, "\r\n", 2);
    }
}

// writes 4 bytes into stdo
// BYTE 1: \x1b escape char or 27 in decimal
// BYTE 2-4: [, 2, and J 
// escape J takes one argument 2 which indicates that the entire screen should clear
void editorRefreshScreen()
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    editorDrawRows();
    
    write(STDOUT_FILENO, "\x1b[H", 3);
}

// init

// initializes global state struct with rows and cols
void initEditor()
{
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
