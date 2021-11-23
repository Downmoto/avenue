#include "terminal.h"

void die(const char *s)
{
    write(STDOUT_FILENO, "\x1b[2J", 4); // clear entire screen
    write(STDOUT_FILENO, "\x1b[H", 3); // send cursor home

    perror(s); // looks at errno and prints s along side errno
    exit(1);
}

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
        die("tcsetattr");
}

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
    raw.c_cc[VMIN] = 0;  // minimum number of bytes before read() can return
    raw.c_cc[VTIME] = 1; // max amount of time read() waits before returns

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr|enableRawMode()");
}

int getCursorPosition(int *rows, int *cols)
{
    char buf[32];
    unsigned int i = 0;

    // write cursor location query command to stdo
    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
        return -1;

    // read query reply from stdi into buf byte by byte
    while (i < sizeof(buf) - 1)
    {
        if (read(STDIN_FILENO, &buf[i], 1) != 1)
            break;
        // when complete, exit loop
        if (buf[i] == 'R')
            break;
        i++;
    }
    buf[i] = '\0'; // null terminate char array

    // check for timeout
    if (buf[0] != '\x1b' || buf[1] != '[')
        return -1;
    
    // write cursor position to pointers, check for failure
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
        return -1;

    return 0;
}

int getWindowsize(int *rows, int *cols)
{
    struct winsize ws; // struct provided by ioctl

    // terminal input/output control get win size. place into ws struct
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
        // if failure occurs, manually get win size by sending cursor to bottom
        // left, then using getCursorPosition to get win size.
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
            return -1;

        return getCursorPosition(rows, cols);
    }
    else
    {
        // on success, set pointers to struct values.
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}