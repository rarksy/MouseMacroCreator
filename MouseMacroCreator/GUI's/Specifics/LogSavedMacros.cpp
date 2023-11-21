#include "../GUI.h"
#include "../../Menu/Menu.h"

bool GUI::Specific::LogSavedMacros::Run(std::vector<std::filesystem::path> macroPaths)
{
    if (macroPaths.empty())
    {
        Menu::Log("No Macros Found!");
        Menu::Log("Macro's Are To Be Placed In The Created 'Macros' Folder");
        Menu::Log("Check The Documentation On The Github Repository For More Info");

        return false;
    }
    char menuOption = '1';
    for (const auto& macro : macroPaths)
    {
        Menu::Log(menuOption, ": ", macro.filename());
        menuOption++;
    }

    return true;
}