#include "Macro.h"
#include <sstream>
#include <algorithm>
#include <ranges>
#include <fstream>
#include "../Menu/Menu.h"

bool MacroCore::IsValidKeyword(std::string line, std::string keyword)
{
    return line.find(keyword) != std::string::npos;
}

bool EnsureValidKeyword(std::string line, std::string& keyword)
{
    if (line.empty())
        return false;

    if (line.find_first_not_of(' ') == ';')
        return false;

    for (const auto& key : MacroCore::validKeywords | std::views::keys)
    {
        if (line.find(key) != std::string::npos)
        {
            keyword = key;
            return true;
        }
    }

    return true;
}

MacroAction MacroCore::ProcessAction(std::istringstream& iss, std::string line)
{
    std::string keyword;

    if (!EnsureValidKeyword(line, keyword))
        return {};

    MacroAction action;

    // Apply The Keyword
    action.keyword = keyword;

    // Find The Keyword In Our Vector To Get The Associated Action Type
    auto it = std::find_if(
        validKeywords.begin(),
        validKeywords.end(),
        [&keyword](const auto& pair)
        {
            return pair.first == keyword;
        });

    action.actionType = it->second;

    const auto actionType2 = action.actionType;

    if (actionType2 == MAT_MouseMove)
    {
        std::pair<int, int> pos;

        if (iss >> action.keyword >> pos.first >> pos.second)
            action.pairArgument = pos;
    }

    else if (actionType2 == MAT_Sleep)
    {
        int sleepAmount;

        if (iss >> action.keyword >> sleepAmount)
            action.intArgument = sleepAmount;
    }

    else if (actionType2 == MAT_MouseDown)
    {
        std::string mouseButton;

        if (iss >> action.keyword >> mouseButton)
            action.stringArgument = mouseButton;
    }

    else if (actionType2 == MAT_MouseUp)
    {
        std::string mouseButton;

        if (iss >> action.keyword >> mouseButton)
            action.stringArgument = mouseButton;
    }

    else if (actionType2 == MAT_KeyDown || actionType2 == MAT_KeyUp)
    {
        std::string key;

        if (iss >> action.keyword >> key)
        {
            if (key.size() == 1)
                action.intArgument = VkKeyScan(key.at(0));

            else
            {
                auto it = std::find_if(validKeys.begin(), validKeys.end(), [&key](const auto& _key)
                {
                    return _key.first == key;
                });

                action.intArgument = it->second;
            }
        }
    }

    return action;
}

void MacroCore::ExecuteAction(const MacroAction& action)
{
    if (action.actionType == MAT_MouseMove)
    {
        SetCursorPos(action.pairArgument.first, action.pairArgument.second);
    }

    else if (action.actionType == MAT_Sleep)
    {
        Sleep(action.intArgument);
    }

    else if (action.actionType == MAT_MouseDown)
    {
        int button;

        if (action.stringArgument == "Left")
            button = MOUSEEVENTF_LEFTDOWN;

        else if (action.stringArgument == "Right")
            button = MOUSEEVENTF_RIGHTDOWN;

        else if (action.stringArgument == "Middle")
            button = MOUSEEVENTF_MIDDLEDOWN;

        mouse_event(button, 0, 0, 0, 0);
    }

    else if (action.actionType == MAT_MouseUp)
    {
        int button;

        if (action.stringArgument == "Left")
            button = MOUSEEVENTF_LEFTUP;

        else if (action.stringArgument == "Right")
            button = MOUSEEVENTF_RIGHTUP;

        else if (action.stringArgument == "Middle")
            button = MOUSEEVENTF_MIDDLEUP;

        mouse_event(button, 0, 0, 0, 0);
    }

    else if (action.actionType == MAT_KeyDown)
    {
        INPUT input;
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;

        input.ki.wVk = action.intArgument;
        input.ki.dwFlags = 0;

        SendInput(1, &input, sizeof INPUT);
    }

    else if (action.actionType == MAT_KeyUp)
    {
        INPUT input;
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;

        input.ki.wVk = action.intArgument;
        input.ki.dwFlags = KEYEVENTF_KEYUP;

        SendInput(1, &input, sizeof INPUT);
    }
}

void MacroCore::RunMacro::Run(const std::filesystem::path& path)
{
    std::ifstream file(path);
    std::string line;
    int lineIndex = 1;

    Macro macro;

    macro.name = path.filename().string();

    while (std::getline(file, line))
    {
        std::istringstream iss(line);

        MacroAction action = ProcessAction(iss, line);

        if (action.keyword.empty())
        {
            std::string invalidKeyword;
            iss >> invalidKeyword;

            Menu::Log(
                path.filename(),
                ": Invalid Keyword (",
                invalidKeyword,
                " ) Found On Line (",
                lineIndex, " )"
            );

            macro.hasError = true;
            continue;
        }

        macro.actions.push_back(action);

        lineIndex++;
    }

    if (macro.hasError)
        return;

    quitMacro = false;

    Menu::Log("Executing ", macro.name);
    Menu::Log("Script Disabled By Default");
    Menu::Log("F8 To Toggle Script");
    Menu::Log("F9 To Terminate Script");

    std::thread macroThread([&macro]
    {
        while (!quitMacro)
        {
            if (!macroRunning)
                continue;

            for (const auto& action : macro.actions)
            {
                if (!macroRunning)
                    break;

                ExecuteAction(action);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });

    while (!quitMacro)
    {
        if (GetAsyncKeyState(VK_F9) & 1)
        {
            quitMacro = true;
            Menu::Log("Terminating Macro...");
        }

        if (GetAsyncKeyState(VK_F8) & 1)
        {
            macroRunning = !macroRunning;
            Menu::Log("Macro ", macroRunning ? "Enabled" : "Disabled");
        }
    }

    macroThread.join();
}
