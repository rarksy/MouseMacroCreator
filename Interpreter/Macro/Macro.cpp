#include "Macro.h"
#include <ranges>
#include <fstream>
#include <regex>

#include "../Menu/Menu.h"

template <typename T>
bool MacroCore::EnsureValidKeyword(std::string& keyword, std::vector<std::pair<std::string, T>> vec)
{
    const int vecSize = vec.size();

    for (int i = 0; i < vecSize; i++)
    {
        const auto key = vec[i].first;
        if (key == keyword)
        {
            Menu::Log("Found Keyword: ", key);
            return true;
        }
    }
    return false;
}

MacroAction MacroCore::ProcessAction(std::istringstream& iss, Macro& macro)
{
    MacroAction action;

    if (iss >> action.keyword && !EnsureValidKeyword(action.keyword, validKeywords))
    {
        action.hasError = true;
        return action;
    }

    // Find The Keyword In Our Vector To Get The Associated Action Type
    const auto it = std::ranges::find_if(
        validKeywords,
        [&action](const auto& pair)
        {
            return pair.first == action.keyword;
        });

    // Apply Appropriate Action Type
    action.actionType = it->second;
    const auto actionType = action.actionType;

    // Process According to MacroActionType (MAT_)

    // Mouse Input
    if (actionType == MAT_SetMousePos)
        MouseInput::SetMousePos::Process(iss, action);

    else if (action.actionType == MAT_SetMousePosInterpolated)
        MouseInput::SetMousePosInterpolated::Process(iss, action);

        // Keyboard Input
    else if (actionType == MAT_MouseDown || actionType == MAT_MouseUp)
        MouseInput::MouseDownUp::Process(iss, action);

    else if (actionType == MAT_MouseClick)
        MouseInput::MouseClick::Process(iss, action);

    else if (actionType == MAT_KeyDown || actionType == MAT_KeyUp)
        KeyboardInput::KeyDownUp::Process(iss, action);

    else if (actionType == MAT_KeyPress)
        KeyboardInput::KeyPress::Process(iss, action);

        // Macro State
    else if (actionType == MAT_SetToggleKey || actionType == MAT_SetTerminateKey)
    {
        MacroState::SetStateKey::Process(iss, actionType == MAT_SetToggleKey ? macro.toggleKey : macro.terminateKey);
        action.isExecutable = false;
    }

    else if (actionType == MAT_SetToggleType)
    {
        MacroState::SetToggleType::Process(iss, macro);
        action.isExecutable = false;
    }

        // Thread Flow
    else if (actionType == MAT_Sleep)
        ThreadFlow::Sleep::Process(iss, action);

    else if (actionType == MAT_Log)
        ThreadFlow::Log::Process(iss, action);

    return action;
}

bool MacroCore::RunMacro::Run(const std::filesystem::path& path)
{
    // Setting Up Our Macro
    Macro macro;
    macro.name = path.filename().string();

    if (!ProcessMacro::Run(path, macro))
    {
        _getch();
        return false;
    }

    // Start Action Execution Loop On New Thread
    // This Keeps Our Main Thread Free And Active

    std::thread macroThread([&macro]
    {
        ExecuteMacro::Run(macro);
    });

    while (!quitMacro)
    {
        if (GetAsyncKeyState(macro.terminateKey) & 1)
        {
            quitMacro = true;
            macroRunning = false;
            macroThread.join();
            return true;
        }

        switch (macro.toggleType)
        {
        case MTT_Toggle:

            if (GetAsyncKeyState(macro.toggleKey) & 1)
                macroRunning = !macroRunning;
            break;

        case MTT_HoldOn:
            if (GetAsyncKeyState(macro.toggleKey))
                macroRunning = true;
            else if (macroRunning)
                macroRunning = false;
            break;

        case MTT_HoldOff:
            if (!GetAsyncKeyState(macro.toggleKey))
                macroRunning = true;
            else if (macroRunning)
                macroRunning = false;
            break;

        case MTT_RunOnce:

            if (macroRunning)
                macroRunning = false;

            if (GetAsyncKeyState(macro.toggleKey) & 1)
                macroRunning = true;
        }
    }

    macroThread.join();
    return true;
}

bool MacroCore::RunMacro::ProcessMacro::Run(const std::filesystem::path& path, Macro& macro)
{
    // Setting Up Our File Reading
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line))
    {
        // Used For Showing Error Line
        static int lineIndex = 1;

        // Read Next Line
        std::istringstream iss(line);

        // Empty Line
        if (line.empty())
            continue;

        // Check If Line Is Comment
        if (line[line.find_first_not_of(' ')] == ';')
            continue;

        // Setup Action
        MacroAction action = ProcessAction(iss, macro);

        // Check If Action Setup Failed
        if (action.hasError)
        {
            // Log Found Error
            Menu::Log(
                path.filename(),
                ": Invalid Keyword ( ",
                action.keyword,
                " ) Found On Line ( ",
                lineIndex, " )"
            );

            return false;
        }

        // Add The Action To Our List To Execute Later

        if (action.isExecutable)
            macro.actions.push_back(action);

        lineIndex++;
    }

    return true;
}

void MacroCore::RunMacro::ExecuteMacro::Run(const Macro& macro)
{
    while (!quitMacro)
    {
        if (!macroRunning)
            continue;

        for (const auto& action : macro.actions)
        {
            if (!macroRunning && macro.toggleType != MTT_RunOnce)
                break;

            // Execute The Macro Action
            ExecuteAction(action);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void MacroCore::ExecuteAction(const MacroAction& action)
{
    // Mouse Input
    if (action.actionType == MAT_SetMousePos)
        MouseInput::SetMousePos::Execute(action);

    else if (action.actionType == MAT_SetMousePosInterpolated)
        MouseInput::SetMousePosInterpolated::Execute(action);

    else if (action.actionType == MAT_MouseDown || action.actionType == MAT_MouseUp)
        MouseInput::MouseDownUp::Execute(action);

    else if (action.actionType == MAT_MouseClick)
        MouseInput::MouseClick::Execute(action);

        // Keyboard Input
    else if (action.actionType == MAT_KeyDown || action.actionType == MAT_KeyUp)
        KeyboardInput::KeyDownUp::Execute(action);

    else if (action.actionType == MAT_KeyPress)
        KeyboardInput::KeyPress::Execute(action);

        // Thread Flow
    else if (action.actionType == MAT_Sleep)
        ThreadFlow::Sleep::Execute(action);

    else if (action.actionType == MAT_Log)
        ThreadFlow::Log::Execute(action);
}
