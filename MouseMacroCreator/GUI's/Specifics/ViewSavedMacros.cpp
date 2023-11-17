#include "../GUI.h"
#include "../../Menu/Menu.h"
#include <fstream>

void GUI::Specific::ViewSavedMacros::Run()
{
    const auto iterator = std::filesystem::directory_iterator(Global::macroPath);
    
    Global::allMacros.clear();
    char macroIndex = '1';
    
    for (auto& entry : iterator)
    {
        std::vector<std::variant<std::pair<int, int>, std::variant<char, int>>> actions;
        std::ifstream file(Global::macroPath.string() + entry.path().filename().string());
        
        if (file.is_open()) {
            std::cout << "opened " << entry.path().filename().string() << std::endl;
            std::string line;
            while (std::getline(file, line))
            {
                std::istringstream iss(line);
                
                
                if (line.size() == 1)
                {
                    char key = line.at(0);
                    actions.push_back(key);
                    std::cout << "found key" << std::endl;
                }
                else if (line.find(' ') != std::string::npos)
                {
                    int x, y;
                    if (iss >> x >> y)
                        actions.push_back(std::pair(x, y));
                    std::cout << "found mousepos" << std::endl;
                }
                else
                {
                    int vk;
                    if (iss >> vk)
                        actions.push_back(vk);

                    std::cout << "found syskey" << std::endl;
                }
            }
            
    }
    
        Macro macro;
        macro.name = entry.path().filename().string();
        macro.actions = actions;
        macro.id = macroIndex;
        
        if (macroIndex == '9')
            macroIndex = 'A';
        
        Global::allMacros.push_back(macro);
        macroIndex++;

        std::cout << "closed " << entry.path().filename().string() << std::endl;
    }
    
    char count = '1';
    for (auto macro : Global::allMacros) {
        Menu::Log(count, ": ", macro.name);
        count++;
    }
    
    char option = Menu::GetKeyPress();
    int optionIndex = option - '1';
    
    GUI::Specific::OpenMacro::Run(Global::allMacros[optionIndex]);
}