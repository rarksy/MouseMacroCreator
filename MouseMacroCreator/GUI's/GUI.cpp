#include "GUI.h"
#include <conio.h>

char GUI::GetInput()
{
    char keyPressed = -1;

    while (char key = _getch())
    {
        if (std::isdigit(key))
        {
            keyPressed = key;
            break;
        }
    }

    return keyPressed;
}
