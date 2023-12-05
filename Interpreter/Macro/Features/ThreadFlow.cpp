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

void MacroCore::MacroState::SetStateKey::Process(std::istringstream& iss, int& stateKey)
{
    std::string keyword;
    std::string toggleKey;

    if (iss >> keyword >> toggleKey)
    {
        if (toggleKey.size() == 1)
        {
            stateKey = VkKeyScan(toggleKey.at(0));
        }

        else
        {
            const auto it = std::ranges::find_if(validKeys, [&toggleKey](const auto& _key)
            {
                return _key.first == toggleKey;
            });

            stateKey = it->second;
        }
    }
}
