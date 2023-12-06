#include "../Macro.h"
#include "../../Menu/Menu.h"

void MacroCore::ThreadFlow::Sleep::Process(std::istringstream& iss, MacroAction& action)
{
    int sleepAmount;

    if (iss >> action.keyword >> sleepAmount)
        action.intArgument = sleepAmount;
}

void MacroCore::ThreadFlow::Sleep::Execute(const MacroAction& action)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(action.intArgument));
}

void MacroCore::ThreadFlow::Log::Process(std::istringstream& iss, MacroAction& action)
{
    std::string logContent;
    
    if (iss >> action.keyword)
        std::getline(iss >> std::ws, action.stringArgument);
}

void MacroCore::ThreadFlow::Log::Execute(const MacroAction& action)
{
    Menu::Log(action.stringArgument);
}