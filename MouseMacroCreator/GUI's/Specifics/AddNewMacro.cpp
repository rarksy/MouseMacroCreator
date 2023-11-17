#include "../GUI.h"
#include "../../Menu/Menu.h"
#include <Windows.h>
#include <fstream>

void GUI::Specific::AddNewMacro::Run()
{
    using namespace Menu;

    Log("Move And Left-Click Mouse To Create Macro");
    Log("MMC Will Not Detect A Click On An Inactive Window");
    Log("Clicking On This Window Will Be Ignored!");
    Log("Press SPACE When Finished");

    std::vector<std::variant<std::pair<int, int>, std::variant<char, int>>> actions;

    char letters[] = {
        'A', 'B', 'C', 'D', 'E', 'F',
        'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z',
    };

    char numbers[] = {
        '0', '1', '2', '3', '4',
        '5', '6', '7', '8', '9'
    };

    int sysKeys[] = {
        VK_CONTROL, VK_SHIFT, VK_MENU, VK_INSERT,
        VK_DELETE, VK_HOME, VK_END, VK_PRIOR, VK_NEXT,
        VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_OEM_COMMA,
        VK_OEM_PERIOD, 
    };

    const HWND consoleWindow = GetForegroundWindow();

    while (GetAsyncKeyState('1'));
    
    while (!(GetAsyncKeyState(VK_SPACE) & 1))
    {
        if ((GetAsyncKeyState(VK_LBUTTON) & 1) && (consoleWindow != GetForegroundWindow()))
        {
            POINT mousePos;
            GetCursorPos(&mousePos);

            actions.push_back(std::pair(mousePos.x, mousePos.y));

            Log("Added X(", mousePos.x, ") Y(", mousePos.y, ")");
        }

        for (auto letter : letters)
        {
            if (GetAsyncKeyState(letter) & 1)
            {
                actions.push_back(letter);
                Log(letter, " Pressed (letter)");   
            }
        }

        for (auto number : numbers)
        {
            if (GetAsyncKeyState(number) & 1)
            {
                actions.push_back(number);
                Log(number, " Pressed (number)");
            }
        }

        for (auto syskey : sysKeys)
        {
            if (GetAsyncKeyState(syskey) & 1)
            {
                actions.push_back(syskey);
                Log(syskey, " Pressed (syskey)");
            }
        }
    }

    Macro macro;
    macro.actions = actions;

    Log("Name The Macro: ");

    std::string macroName;
    std::cin >> macroName;

    macro.name = macroName;

    // Save Macro To File
    std::filesystem::path filePath(Global::macroPath.string() + macroName + ".macro");
    std::cout << filePath.string();

    std::ofstream file(filePath);

    if (file.is_open())
    {
        for (const auto& action : actions)
        {
            if (auto pos = std::get_if<std::pair<int, int>>(&action))
                file << pos->first << " " << pos->second << std::endl;

            else if (auto variant = std::get_if<std::variant<char, int>>(&action))
            {
                if (auto charKey = std::get_if<char>(variant))
                    file << *charKey << std::endl;

                else if (auto intKey = std::get_if<int>(variant))
                    file << *intKey << std::endl;
            }
        }
        file.close();

        Log("Saved Macro!");
    }
    else
        Log("Error Saving Macro.");
}
