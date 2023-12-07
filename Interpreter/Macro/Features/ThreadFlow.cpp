#include "../Macro.h"
#include "../../Menu/Menu.h"

void MacroCore::ThreadFlow::Sleep::Process(std::istringstream& iss, MacroAction& action)
{
    iss >> action.intArgument;
}

void MacroCore::ThreadFlow::Sleep::Execute(const MacroAction& action)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(action.intArgument));
}

void MacroCore::ThreadFlow::Log::Process(std::istringstream& iss, MacroAction& action)
{
    std::getline(iss >> std::ws, action.stringArgument);
}

void MacroCore::ThreadFlow::Log::Execute(const MacroAction& action)
{
    std::cout << action.stringArgument << std::endl;
}