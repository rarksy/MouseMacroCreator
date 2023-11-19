#include "../GUI.h"
#include "../../Menu/Menu.h"
#include <fstream>
#include <Windows.h>

void GUI::Specific::ViewSavedMacros::Run()
{
    const auto iterator = std::filesystem::directory_iterator(Global::macroPath);

    std::vector<Macro> AllMacros;

    char macroIndex = '1';

    for (auto& entry : iterator)
    {
        std::ifstream file(Global::macroPath.string() + entry.path().filename().string());
        Macro macro;

        if (file.is_open())
        {
            std::string line;
            int lineIndex = 1;
            while (std::getline(file, line))
            {
                std::istringstream iss(line);

                if (line.empty())
                    continue;

                if (line.at(line.find_first_not_of(' ')) == ';')
                    continue;

                MacroAction action = MacroCore::ProcessAction(iss, line);

                if (action.keyword.empty())
                {
                    std::string invalidKeyword;
                    iss >> invalidKeyword;

                    Menu::Log(entry.path().filename().string(), ": Invalid Keyword ( ", invalidKeyword,
                    " ) Found On Line ( ", lineIndex, " )");
                    macro.hasError = true;
                    continue;
                }

                macro.actions.push_back(action);

                lineIndex++;
            }
        }

        if (macroIndex == '9')
            macroIndex = 'A';

        macro.name = entry.path().filename().string();
        macro.id = macroIndex;
        macroIndex++;

        AllMacros.push_back(macro);
    }

    for (auto& macro : AllMacros)
        Menu::Log(macro.id, ": ", macro.name);

    char option;
    ZeroMemory(&option, sizeof option);


GetMacroChoice:

    Menu::GetKeyPress(option);
    int optionIndex = option - '1';

    if (optionIndex >= 0 && optionIndex < AllMacros.size())
        RunMacro::Run(AllMacros.at(optionIndex));
    else
        goto GetMacroChoice;
}
