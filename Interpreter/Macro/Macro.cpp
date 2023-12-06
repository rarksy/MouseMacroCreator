#include "Macro.h"
#include <ranges>
#include <fstream>
#include <regex>

#include "../Menu/Menu.h"

template<typename T>
bool MacroCore::EnsureValidKeyword(const std::string& line, std::string& keyword, std::vector<std::pair<std::string, T>> vec)
{

    for (const auto& key : vec | std::views::keys)
    {
        std::string pattern = "\\b" + key + "\\b";

        if (std::regex_search(line, std::regex(pattern)))
        {
            keyword = key;
            Menu::Log("Found Keyword: ", keyword);
            return true;
        }
    }

    return false;
}

MacroAction MacroCore::ProcessAction(std::istringstream& iss, const std::string& line, Macro& macro)
{
    // Temp Keyword Variable
    std::string keyword;

    // Make Sure Keyword Exists In Our Array
    if (!EnsureValidKeyword(line, keyword, validKeywords))
        return {};
    
    MacroAction action;
    action.keyword = keyword;

    // Find The Keyword In Our Vector To Get The Associated Action Type
    const auto it = std::ranges::find_if(
        validKeywords,
        [&keyword](const auto& pair)
    {
            return pair.first == keyword;
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
        MacroState::SetStateKey::Process(iss, actionType == MAT_SetToggleKey ? macro.toggleKey : macro.terminateKey);

    else if (actionType == MAT_SetToggleType)
        MacroState::SetToggleType::Process(iss, macro);

    // Thread Flow
    else if (actionType == MAT_Sleep)
        ThreadFlow::Sleep::Process(iss, action);

    else if (actionType == MAT_Log)
        ThreadFlow::Log::Process(iss, action);
    
    return action;
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

bool MacroCore::RunMacro::Run(const std::filesystem::path& path)
{
    // Setting Up Our Macro
    Macro macro;
    macro.name = path.filename().string();

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
        MacroAction action = ProcessAction(iss, line, macro);

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
            break;   
        }

        // Add The Action To Our List To Execute Later
        macro.actions.push_back(action);

        lineIndex++;
    }

    if (macro.hasError)
    {
        _getch();
        return false;   
    }

    quitMacro = false;

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
                if (!macroRunning && macro.toggleType != MTT_RunOnce)
                    break;

                // Execute The Macro Action
                ExecuteAction(action);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });

    while (!quitMacro)
    {
        if (GetAsyncKeyState(macro.terminateKey) & 1)
        {
            quitMacro = true;
            macroRunning = false;
            Menu::Log("Terminating Macro...");
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
