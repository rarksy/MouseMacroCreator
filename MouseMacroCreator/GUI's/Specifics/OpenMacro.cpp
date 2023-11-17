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
 
void GUI::Specific::OpenMacro::Run(Macro macro)
{
    int runCount = 0;
    int maxRunCount = 0;
    std::cout << "How Many Times To Run: ";
    std::cin >> maxRunCount;
    int actionDelayMS = 0;
    std::cout << "\nDelay In MS: ";
    std::cin >> actionDelayMS;
    while (runCount < maxRunCount)
    {
        for (const auto& action : macro.actions)
        {
            if (const auto pos = std::get_if<std::pair<int, int>>(&action))
            {
                SetCursorPos(pos->first, pos->second);
                Menu::Log("Moved Mouse");
                Sleep(actionDelayMS);
                LeftClick();
                Menu::Log("Clicked");
                Sleep(actionDelayMS);
            }
            else if (auto variant = std::get_if<std::variant<char, int>>(&action))
            {
                if (const auto key = std::get_if<char>(variant))
                {
                    KeyPress(*key);
                    Menu::Log("Pressed ", key);
                    Sleep(actionDelayMS);
                }
                
                else if (const auto vk = std::get_if<int>(variant))
                {
                    KeyPress((int)vk);
                    Menu::Log("Pressed ", vk);
                    Sleep(actionDelayMS);
                }
            }
        }

        runCount++;
    }
}
