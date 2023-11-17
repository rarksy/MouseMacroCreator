#pragma once
#include <conio.h>
#include <filesystem>
#include <iostream>
#include <variant>

namespace Menu
{
    template <typename... Args>
    void Log(Args&&... args)
    {
        (std::cout << ... << std::forward<Args>(args)) << std::endl;
    }

    inline void Clear()
    {
        system("cls");
    }

    inline char GetKeyPress()
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
}

namespace Global
{
    inline std::filesystem::path macroPath;
}