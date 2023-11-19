#include "Macro.h"
#include <sstream>
#include <algorithm>
#include <ranges>


bool MacroCore::IsValidKeyword(std::string line, std::string keyword)
{
    return line.find(keyword) != std::string::npos;
}

MacroAction MacroCore::ProcessAction(std::istringstream& iss, std::string line)
{
    std::string keyword;
    for (const auto& key : validKeywords)
    {
        if (!IsValidKeyword(line, key.first))
            continue;
        
        keyword = key.first;
        break;
    }

    if (keyword.empty())
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
        char key;

        if (iss >> action.keyword >> key)
            action.intArgument = VkKeyScanW(key);
    }
    
    return action;
}
