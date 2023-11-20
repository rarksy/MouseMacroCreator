#include "../Macro.h"

void MacroCore::MouseInput::SetMousePos::Process(std::istringstream& iss, MacroAction& action)
{
    std::pair<int, int> pos;

    if (iss >> action.keyword >> pos.first >> pos.second)
        action.pairArgument = pos;
}

void MacroCore::MouseInput::SetMousePos::Execute(const MacroAction& action)
{
    SetCursorPos(action.pairArgument.first, action.pairArgument.second);
}

void MacroCore::MouseInput::MouseDownUp::Process(std::istringstream& iss, MacroAction& action)
{
    std::string mouseButton;

    if (iss >> action.keyword >> mouseButton)
        action.stringArgument = mouseButton;
}

void MacroCore::MouseInput::MouseDownUp::Execute(const MacroAction& action)
{
    int button;

    const auto it = std::find_if(validMouseButtons.begin(), validMouseButtons.end(), [&action](const auto& pair)
    {
        return pair.first == action.stringArgument;
    });
    
    if (it != validMouseButtons.end())
        button = action.actionType == MAT_MouseDown ? it->second.first : it->second.second;
    
    mouse_event(button, 0, 0, 0, 0);
}

void MacroCore::MouseInput::MouseClick::Process(std::istringstream& iss, MacroAction& action)
{
    MouseDownUp::Process(iss, action);
}

void MacroCore::MouseInput::MouseClick::Execute(const MacroAction& action)
{
    auto actionCopy = action;
    actionCopy.actionType = MAT_MouseDown;
    MouseDownUp::Execute(actionCopy);
    actionCopy.actionType = MAT_MouseUp;
    MouseDownUp::Execute(actionCopy);
}