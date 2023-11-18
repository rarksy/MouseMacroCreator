
// console interface
// save via file
// list all saved files as executable macros
// ability to run / open in text editor / delete macro

#include <filesystem>
#include <Windows.h>
#include "Menu/Menu.h"
#include "GUI's/GUI.h"

using namespace Menu;

int main()
{
    // Check For Macro Directory
    std::string currentPath = std::filesystem::current_path().string();
    std::filesystem::path macroPath = currentPath + "\\Macros\\";

    if (!exists(macroPath))
    {
        if (create_directory(macroPath))
            MessageBoxA(NULL, "Macro Directory Created!", "Notice...", MB_OK);
    }

    Global::macroPath = macroPath;

    while (true)
    {
        GUI::Specific::ViewSavedMacros::Run();
    }
    
    return 0;
}