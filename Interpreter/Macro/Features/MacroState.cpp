#include "../Macro.h"

void MacroCore::MacroState::SetStateKey::Process(std::istringstream& iss, int& stateKey)
{
    std::string toggleKey;

    if (iss >> toggleKey)
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

            if (it != validKeys.end())
            stateKey = it->second;
        }
    }
}

void MacroCore::MacroState::SetToggleType::Process(std::istringstream& iss, Macro& macro)
{
    std::string toggleType;

    if (iss >> toggleType)
    {
        const auto it = std::ranges::find_if(validToggleTypes, [&toggleType](const auto& type)
        {
            return type.first == toggleType;
        });

        if (it != validToggleTypes.end())
            macro.toggleType = it->second;
    }
}
