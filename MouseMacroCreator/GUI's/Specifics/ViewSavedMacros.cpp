#include "../GUI.h"
#include "../../Menu/Menu.h"
#include <fstream>

bool ValidKeyword(std::string line, std::string keyword)
{
    return (line.find(keyword) != std::string::npos);
}

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
                MacroAction action;
                std::istringstream iss(line);

                if (ValidKeyword(line, "SetMousePos"))
                {
                    action.actionType = MAT_MouseMove;
                    std::pair<int, int> pos;

                    if (iss >> action.keyword >> pos.first >> pos.second)
                        action.pairArgument = pos;
                }

                else if (ValidKeyword(line, "Sleep"))
                {
                    action.actionType = MAT_Sleep;
                    int sleepAmount;
                    
                    if (iss >> action.keyword >> sleepAmount)
                        action.intArgument = sleepAmount;
                }


                else
                {
                    std::string invalidKeyword;
                    iss >> invalidKeyword;
                    
                    Menu::Log("Invalid Keyword ( ", invalidKeyword, " ) Found On Line ( ", lineIndex, " )");
                    RunMacro::quitMacro = true;
                }

                lineIndex++;
                macro.actions.push_back(action);
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

    char option = Menu::GetKeyPress();
    int optionIndex = option - '1';

    RunMacro::Run(AllMacros.at(optionIndex));
}
