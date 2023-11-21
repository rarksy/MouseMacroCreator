#include "../Macro.h"

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