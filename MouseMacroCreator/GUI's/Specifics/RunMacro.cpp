#include <Windows.h>
#include "../GUI.h"
#include "../../Menu/Menu.h"

void GUI::Specific::RunMacro::Run(Macro macro)
{
    if (macro.hasError)
        return;

    quitMacro = false;

    Menu::Log("Executing ", macro.name);
    Menu::Log("Script Disabled By Default, Press F8 To Toggle");
    Menu::Log("Press F9 To Terminate Script");

    std::thread macroThread([&macro]
    {
        while (!quitMacro)
        {
            if (!macroRunning)
                continue;

            for (const auto& action : macro.actions)
            {
                if (!macroRunning)
                    break;
                
                MacroActionType actionType = action.actionType;

                if (actionType == MAT_MouseMove)
                {
                    SetCursorPos(action.pairArgument.first, action.pairArgument.second);
                }

                else if (actionType == MAT_Sleep)
                {
                    Sleep(action.intArgument);
                }

                else if (actionType == MAT_MouseDown)
                {
                    int button;

                    if (action.stringArgument == "Left")
                        button = MOUSEEVENTF_LEFTDOWN;

                    else if (action.stringArgument == "Right")
                        button = MOUSEEVENTF_RIGHTDOWN;

                    else if (action.stringArgument == "Middle")
                        button = MOUSEEVENTF_MIDDLEDOWN;

                    mouse_event(button, 0, 0, 0, 0);
                }

                else if (actionType == MAT_MouseUp)
                {
                    int button;

                    if (action.stringArgument == "Left")
                        button = MOUSEEVENTF_LEFTUP;

                    else if (action.stringArgument == "Right")
                        button = MOUSEEVENTF_RIGHTUP;

                    else if (action.stringArgument == "Middle")
                        button = MOUSEEVENTF_MIDDLEUP;

                    mouse_event(button, 0, 0, 0, 0);
                }

                else if (actionType == MAT_KeyDown)
                {
                    INPUT input;
                    input.type = INPUT_KEYBOARD;
                    input.ki.wScan = 0;
                    input.ki.time = 0;
                    input.ki.dwExtraInfo = 0;

                    input.ki.wVk = action.intArgument;
                    input.ki.dwFlags = 0;

                    SendInput(1, &input, sizeof INPUT);
                }

                else if (actionType == MAT_KeyUp)
                {
                    INPUT input;
                    input.type = INPUT_KEYBOARD;
                    input.ki.wScan = 0;
                    input.ki.time = 0;
                    input.ki.dwExtraInfo = 0;

                    input.ki.wVk = action.intArgument;
                    input.ki.dwFlags = KEYEVENTF_KEYUP;

                    SendInput(1, &input, sizeof INPUT);
                }
            }
        }
    });
    macroThread.detach();

    while (!quitMacro)
    {
        if (GetAsyncKeyState(VK_F9) & 1)
        {
            quitMacro = true;
            Menu::Log("Terminating Macro...");
        }

        if (GetAsyncKeyState(VK_F8) & 1)
        {
            macroRunning = !macroRunning;
            Menu::Log("Macro ", macroRunning ? "Enabled" : "Disabled");
        }
    }
}