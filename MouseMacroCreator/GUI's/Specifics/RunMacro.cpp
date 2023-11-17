#include <Windows.h>

#include "../GUI.h"
#include "../../Menu/Menu.h"

auto LeftClick = []
{
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
};

void KeyPress(char key)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;

    input.ki.wVk = VkKeyScan(key) & 0xFF;
    input.ki.dwFlags = 0;
    SendInput(1, &input, sizeof(INPUT));

    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void GUI::Specific::RunMacro::Run(Macro macro)
{
    if (quitMacro)
        return;
    
    Menu::Log("Executing ", macro.name);
    Menu::Log("Script Disabled By Default, Press F8 To Toggle");
    Menu::Log("Press F9 + F10 To Terminate Script");

    while (!quitMacro)
    {
        if ((GetAsyncKeyState(VK_F9) & 1) && (GetAsyncKeyState(VK_F10) & 1))
            quitMacro = true;

        if (GetAsyncKeyState(VK_F8) & 1)
        {
            macroRunning = !macroRunning;
            Menu::Log("Macro ", macroRunning ? "Enabled" : "Disabled");
        }

        if (!macroRunning)
            continue;

        for (const auto& action : macro.actions)
        {
            switch (action.actionType)
            {
                
            case MAT_MouseMove:

                SetCursorPos(action.pairArgument.first, action.pairArgument.second);
                
                break;

            case MAT_Sleep:

                Sleep(action.intArgument);
                
                break;
                
            }
        }
    }
}