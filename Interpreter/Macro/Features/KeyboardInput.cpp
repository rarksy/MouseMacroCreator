#include "../Macro.h"

void MacroCore::KeyboardInput::KeyDownUp::Process(std::istringstream& iss, MacroAction& action)
{
    std::string key;

    if (iss >> action.keyword >> key)
    {
        if (key.size() == 1)
        {
            if (isupper(key[0]))
                action.keyPressToUpper = true;

            action.intArgument = VkKeyScan(key.at(0));
        }

        else
        {
            const auto it = std::ranges::find_if(validKeys, [&key](const auto& _key)
            {
               return _key.first == key; 
            });

            action.intArgument = it->second;
        }
    }
}

void MacroCore::KeyboardInput::KeyDownUp::Execute(const MacroAction& action)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = action.intArgument;
    input.ki.dwFlags = action.actionType == MAT_KeyUp ? KEYEVENTF_KEYUP : 0;
    
    INPUT shiftInput = input;
    shiftInput.ki.wVk = VK_SHIFT;

    if (action.keyPressToUpper)
        SendInput(1, &shiftInput, sizeof INPUT);

    SendInput(1, &input, sizeof INPUT);
}

void MacroCore::KeyboardInput::KeyPress::Process(std::istringstream& iss, MacroAction& action)
{
    KeyDownUp::Process(iss, action);
}

void MacroCore::KeyboardInput::KeyPress::Execute(const MacroAction& action)
{
    auto actionCopy = action;
    actionCopy.actionType = MAT_KeyDown;
    KeyDownUp::Execute(actionCopy);
    actionCopy.actionType = MAT_KeyUp;
    KeyDownUp::Execute(actionCopy);
    
}