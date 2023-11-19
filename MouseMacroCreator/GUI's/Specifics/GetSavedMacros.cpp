#include "../GUI.h"
#include "../../Menu/Menu.h"
#include <fstream>
#include <Windows.h>

std::vector<std::filesystem::path> GUI::Specific::GetSavedMacros::Get()
{
    const auto iterator = std::filesystem::directory_iterator(Global::macroPath);

    std::vector<std::filesystem::path> macroPaths;
    
    char macroIndex = '0';

    for (auto& entry : iterator)
    {
        std::ifstream file(Global::macroPath.string() + entry.path().filename().string());

        if (!file.is_open())
        {
            Menu::Log("An Error Occured Opening File: ", entry.path().filename());
            continue;
        }

        if (entry.path().extension() != ".mmc")
            continue;

        macroPaths.push_back(entry.path());
        macroIndex++;
    }

    return macroPaths;
    
}
