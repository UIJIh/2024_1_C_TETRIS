#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <windows.h>

// Enum for cursor types
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

void gotoxy(int x, int y);
void setcursortype(CURSOR_TYPE c);
void textcolor(int colorNum);

#endif // WINDOWMANAGER_H
