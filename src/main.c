#include "editor.h"

int main(int argc, char* argv[])
{
    initEditor();

    if (argc >= 2)
        editorOpen(argv[1]);

    while (1)
    {
        editorRefreshScreen();
        editorProcessKeypress();
    }

    return 0;
}
