#include <filesystem>
#include <Windows.h>
#include "Menu/Menu.h"
#include "GUI's/GUI.h"
#include "Macro/Macro.h"

int main()
{
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
    }

    while (true)
    {
        auto macroPaths = GUI::Specific::GetSavedMacros::Get();

        char option;
        int optionIndex;
        ZeroMemory(&option, sizeof option);

        if (!GUI::Specific::LogSavedMacros::Run(macroPaths))
        {
            std::this_thread::sleep_for(std::chrono::seconds(20));
            break;
        }


        while (option < 1)
        {
            Menu::GetKeyPress(option);
            optionIndex = option - '1';

            if (optionIndex < macroPaths.size() && optionIndex > 0)
                break;
        }

        MacroCore::RunMacro::Run(macroPaths.at(optionIndex));
    }

    return 0;
}
