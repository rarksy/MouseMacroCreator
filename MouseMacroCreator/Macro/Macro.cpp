#include "Macro.h"
#include <algorithm>
#include <ranges>
#include <fstream>
#include "../Menu/Menu.h"

bool MacroCore::EnsureValidKeyword(std::string line, std::string& keyword)
{
    for (const auto& key : validKeywords | std::views::keys)
    {
        if (line.find(key) != std::string::npos)
        {
            keyword = key;
            return true;
        }
    }

    return false;
}

MacroAction MacroCore::ProcessAction(std::istringstream& iss, std::string line)
{
    std::string keyword;

    // Make Sure Keyword Exists In Our Array
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

    const auto actionType = action.actionType;

    // Process According to MacroActionType (MAT_)

    if (actionType == MAT_MouseMove)
        MouseInput::SetMousePos::Process(iss, action);

    else if (actionType == MAT_Sleep)
        ThreadFlow::Sleep::Process(iss, action);

    else if (actionType == MAT_MouseDown || actionType == MAT_MouseUp)
        MouseInput::MouseDownUp::Process(iss, action);

    else if (actionType == MAT_MouseClick)
        MouseInput::MouseClick::Process(iss, action);

    else if (actionType == MAT_KeyDown || actionType == MAT_KeyUp)
        KeyboardInput::KeyDownUp::Process(iss, action);

    else if (actionType == MAT_KeyPress)
        KeyboardInput::KeyPress::Process(iss, action);

    return action;
}

void MacroCore::ExecuteAction(const MacroAction& action)
{
    // Mouse Input

    if (action.actionType == MAT_MouseMove)
        MouseInput::SetMousePos::Execute(action);

    else if (action.actionType == MAT_MouseDown || action.actionType == MAT_MouseUp)
        MouseInput::MouseDownUp::Execute(action);

    else if (action.actionType == MAT_MouseClick)
        MouseInput::MouseClick::Execute(action);

    // Thread Flow

    else if (action.actionType == MAT_Sleep)
        ThreadFlow::Sleep::Execute(action);

    // Keyboard Input

    else if (action.actionType == MAT_KeyDown || action.actionType == MAT_KeyUp)
        KeyboardInput::KeyDownUp::Execute(action);

    else if (action.actionType == MAT_KeyPress)
        KeyboardInput::KeyPress::Execute(action);
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
        // Read Next Line
        std::istringstream iss(line);

        // Empty Line
        if (line.empty())
            continue;

        // Check If Line Is Comment
        if (line[line.find_first_not_of(' ')] == ';')
            continue;

        // Setup Action
        MacroAction action = ProcessAction(iss, line);

        // Check If Action Setup Failed
        if (action.keyword.empty())
        {
            std::string invalidKeyword;
            iss >> invalidKeyword;

            // Log
            Menu::Log(
                path.filename(),
                ": Invalid Keyword ( ",
                invalidKeyword,
                " ) Found On Line ( ",
                lineIndex, " )"
            );

            // Mark The Macro As Having An Error
            macro.hasError = true;
            continue;
        }

        // Add The Action To Our List To Execute Later
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

    // Start Action Execution Loop On New Thread
    // This Keeps Our GUI Loop Free And Active

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

                // Execute The Macro Action
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
