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

struct Macro
{
    std::vector<std::variant<std::pair<int, int>, std::variant<char, int>>> actions;
    std::string name;
    char id;
};

namespace Global
{
    inline std::filesystem::path macroPath;

    inline std::vector<Macro> allMacros;
    
}