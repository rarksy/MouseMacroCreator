
// console interface
// save via file
// list all saved files as executable macros
// ability to run / open in text editor / delete macro

#include <filesystem>
#include <conio.h>
#include <functional>
#include <Windows.h>
#include <string>

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
        Log("MMC By Rarksy / ski");
        Log("");
        Log("Choose An Option");
        Log("");
        Log('1', ": ", "Add New Macro");
        Log('2', ": ", "View Saved Macro's");
        

        switch (GetKeyPress())
        {
        case '1':
            Clear();
            GUI::Specific::AddNewMacro::Run();
            break;

        case '2':
            Clear();
            GUI::Specific::ViewSavedMacros::Run();
            break;
        }
    }
    
    return 0;
}