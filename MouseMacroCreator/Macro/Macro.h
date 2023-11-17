#pragma once
#include <vector>
#include <string>

enum MacroActionType
{
    MAT_KeyDown,
    MAT_KeyUp,
    MAT_MouseMove,
    MAT_MouseClick,

    MAT_Sleep
};

struct MacroAction
{
    MacroActionType actionType;
    std::string keyword;

    int intArgument;
    char charArgument;
    std::pair<int, int> pairArgument;
};

struct Macro
{
    std::vector<MacroAction> actions;
    std::string name;
    char id;
};

namespace MacroCore
{
    bool IsValidKeyword(std::string line, std::string keyword);


    inline std::vector<std::string> validKeywords {

        "SetMousePos",
        "Sleep",

    };
}