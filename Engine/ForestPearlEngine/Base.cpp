#include <stdio.h>
#include <windows.h>
#include <conio.h>


void gotoxy(int x, int y)
{
    COORD Cur;
    Cur.X = x;
    Cur.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur); //커서 위치 변경
}

int x = 0;
int y = 0;

void ProcessInput()
{
    if (GetAsyncKeyState(VK_UP) & 0x8000) y--;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) y++;
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) x--;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) x++;
}

int main(void)
{
    putchar('@');

    while (true)
    {
        Sleep(100);
        ProcessInput();

        gotoxy(x, y);
        putchar('@');    //현재 커서 위치에 달팽이를 그려요    
    }


    return 0;
}