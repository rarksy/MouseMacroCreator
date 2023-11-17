#pragma once
#include <functional>
#include <string>
#include <vector>

struct Macro;

namespace GUI
{
    char GetInput();
    
    namespace MainMenu
    {
        char Run();
        void CreateOption(std::vector<std::pair<char, std::string>> options, char key, std::string option);
        void ProcessMenuOption(char key);
        
        inline std::vector<std::pair<char, std::string>> menuOptions;
    }

    namespace Specific
    {
        namespace AddNewMacro
        {
            void Run();

            inline std::vector<std::pair<char, std::string>> menuOptions;
        }

        namespace ViewSavedMacros
        {
            void Run();
        }

        namespace OpenMacro
        {
            void Run(Macro macro);
        }
    }
}
