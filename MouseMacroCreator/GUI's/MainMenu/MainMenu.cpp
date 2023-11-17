#include "../GUI.h"
#include "../../Menu/Menu.h"

char GUI::MainMenu::Run()
{
    menuOptions.clear();
    
    using namespace Menu;

    Log("MMC By Rarksy / ski");
    Log("");
    Log("Choose An Option");
    Log("");

    CreateOption(menuOptions, '1', "Add New Macro");

    return GUI::GetInput();
}

void GUI::MainMenu::CreateOption(std::vector<std::pair<char, std::string>> options, char key, std::string option)
{
    Menu::Log(key, ": ", option);

    options.push_back(std::pair(key, option));
}

void GUI::MainMenu::ProcessMenuOption(char key)
{
    switch (key)
    {
    case '1':
        Specific::AddNewMacro::Run();
        break;
    }
}
