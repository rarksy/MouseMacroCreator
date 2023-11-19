
//t0d0
// add documentation
// add modifier key support to KeyDown/Up

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