#pragma once
#include <conio.h>
#include <filesystem>
#include <iostream>

namespace Menu
{
    template <typename... Args>
    void Log(Args&&... args)
    {
#ifdef _DEBUG
        (std::cout << ... << std::forward<Args>(args)) << std::endl;
#endif
    }
}

namespace Global
{
    inline std::filesystem::path macroPath;
}