#pragma once
#include <conio.h>
#include <filesystem>
#include <iostream>

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

    inline void GetKeyPress(char& storedChar)
    {
        while (char key = _getch())
        {
            if (std::isdigit(key))
            {
                storedChar = key;
                break;
            }
        }
    }
}

namespace Global
{
    inline std::filesystem::path macroPath;
}