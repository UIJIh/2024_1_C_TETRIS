#include "WindowManager.h"

void gotoxy(int x, int y) { // gotoxy �Լ�
    COORD pos = { 2 * x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setcursortype(CURSOR_TYPE c) { // Ŀ�� ����� �Լ�
    CONSOLE_CURSOR_INFO CurInfo;

    switch (c) {
    case NOCURSOR:
        CurInfo.dwSize = 1;
        CurInfo.bVisible = FALSE;
        break;
    case SOLIDCURSOR:
        CurInfo.dwSize = 100;
        CurInfo.bVisible = TRUE;
        break;
    case NORMALCURSOR:
        CurInfo.dwSize = 20;
        CurInfo.bVisible = TRUE;
        break;
    }
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void textcolor(int colorNum) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}
