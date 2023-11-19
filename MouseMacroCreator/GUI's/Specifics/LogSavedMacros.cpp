#include "../GUI.h"
#include "../../Menu/Menu.h"

void GUI::Specific::LogSavedMacros::Run(std::vector<std::filesystem::path> macroPaths)
{
    char menuOption = '1';
    for (const auto& macro : macroPaths)
    {
        Menu::Log(menuOption, ": ", macro.filename());
        menuOption++;
    }
}