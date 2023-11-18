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
                MacroAction action;
                std::istringstream iss(line);

                if (line.empty())
                    continue;
                
                if (line.at(0) == ';')
                    continue;

                if (MacroCore::IsValidKeyword(line, "SetMousePos"))
                {
                    action.actionType = MAT_MouseMove;
                    std::pair<int, int> pos;

                    if (iss >> action.keyword >> pos.first >> pos.second)
                        action.pairArgument = pos;
                }

                else if (MacroCore::IsValidKeyword(line, "Sleep"))
                {
                    action.actionType = MAT_Sleep;
                    int sleepAmount;

                    if (iss >> action.keyword >> sleepAmount)
                        action.intArgument = sleepAmount;
                }

                else if (MacroCore::IsValidKeyword(line, "MouseDown"))
                {
                    action.actionType = MAT_MouseDown;
                    std::string mouseButton;

                    if (iss >> action.keyword >> mouseButton)
                        action.stringArgument = mouseButton;
                }

                else if (MacroCore::IsValidKeyword(line, "MouseUp"))
                {
                    action.actionType = MAT_MouseUp;
                    std::string mouseButton;

                    if (iss >> action.keyword >> mouseButton)
                        action.stringArgument = mouseButton;
                }

                else if (MacroCore::IsValidKeyword(line, "KeyDown") ||
                    MacroCore::IsValidKeyword(line, "KeyUp"))
                {
                    action.actionType = MAT_KeyDown;
                    char key;

                    if (iss >> action.keyword >> key)
                        action.intArgument = VkKeyScanW(key);
                }


                else
                {
                    std::string invalidKeyword;
                    iss >> invalidKeyword;

                    Menu::Log(entry.path().filename().string(), ": Invalid Keyword ( ", invalidKeyword,
                              " ) Found On Line ( ", lineIndex, " )");
                    macro.hasError = true;
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

    char option;
    ZeroMemory(&option, sizeof option);


GetMacroChoice:

    Menu::GetKeyPress(option); // not getting user input after first time
    int optionIndex = option - '1';

    if (optionIndex >= 0 && optionIndex < AllMacros.size())
        RunMacro::Run(AllMacros.at(optionIndex));
    else
        goto GetMacroChoice;
}
