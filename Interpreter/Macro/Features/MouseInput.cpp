#include "../Macro.h"
#include "../../Menu/Menu.h"

void MacroCore::MouseInput::SetMousePos::Process(std::istringstream& iss, MacroAction& action)
{
    std::pair<int, int> pos;

    if (iss >> pos.first >> pos.second)
        action.pairArgument = pos;
}

void MacroCore::MouseInput::SetMousePos::Execute(const MacroAction& action)
{
    SetCursorPos(action.pairArgument.first, action.pairArgument.second);
}

void MacroCore::MouseInput::SetMousePosInterpolated::Process(std::istringstream& iss, MacroAction& action)
{
    std::pair<int, int> pos;
    int timeToComplete;

    if (iss >> pos.first >> pos.second >> timeToComplete)
    {
        action.pairArgument = pos;
        action.intArgument = timeToComplete;
    }
}

void MacroCore::MouseInput::SetMousePosInterpolated::Execute(const MacroAction& action) // rework at some point
{
    POINT cursorPos;
    GetCursorPos(&cursorPos);

    const int targetX = action.pairArgument.first;
    const int targetY = action.pairArgument.second;

    const int totalDuration = action.intArgument;
    
    const int deltaX = targetX - cursorPos.x;
    const int deltaY = targetY - cursorPos.y;
    
    const double totalDistance = std::hypot(deltaX, deltaY);
    
    const double speed = totalDistance / totalDuration;
    
    const auto startTime = std::chrono::high_resolution_clock::now();
    
    for (int elapsed = 0; elapsed <= totalDuration; elapsed++)
    {
        MacroAction actionCopy = action;
        
        actionCopy.pairArgument.first = static_cast<int>(cursorPos.x + (elapsed * speed * deltaX / totalDistance));
        actionCopy.pairArgument.second = static_cast<int>(cursorPos.y + (elapsed * speed * deltaY / totalDistance));

        SetMousePos::Execute(actionCopy);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    const auto endTime = std::chrono::high_resolution_clock::now();
    const auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    if (elapsedTime < totalDuration)
        std::this_thread::sleep_for(std::chrono::milliseconds(totalDuration - elapsedTime));
    
}


void MacroCore::MouseInput::MouseDownUp::Process(std::istringstream& iss, MacroAction& action)
{
    std::string mouseButton;

    if (iss >> mouseButton)
        action.stringArgument = mouseButton;

    if (action.stringArgument.find("Thumb1") != std::string::npos)
        action.intArgument = XBUTTON1;

    if (action.stringArgument.find("Thumb2") != std::string::npos)
        action.intArgument = XBUTTON2;
}

void MacroCore::MouseInput::MouseDownUp::Execute(const MacroAction& action)
{
    int button;

    const auto it = std::ranges::find_if(validMouseButtons, [&action](const auto& pair)
    {
        return pair.first == action.stringArgument;
    });

    if (it != validMouseButtons.end())
        button = action.actionType == MAT_MouseDown ? it->second.first : it->second.second;

    mouse_event(button, 0, 0, action.intArgument, 0);
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
