#include "../Macro.h"

void MacroCore::ThreadFlow::Sleep::Process(std::istringstream& iss, MacroAction& action)
{
    int sleepAmount;

    if (iss >> action.keyword >> sleepAmount)
        action.intArgument = sleepAmount;
}

void MacroCore::ThreadFlow::Sleep::Execute(const MacroAction& action)
{
    ::Sleep(action.intArgument);
}