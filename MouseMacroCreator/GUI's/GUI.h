#pragma once
#include <filesystem>

#include "../Macro/Macro.h"

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

        namespace GetSavedMacros
        {
            std::vector<std::filesystem::path> Get();
        }

        namespace LogSavedMacros
        {
            void Run(std::vector<std::filesystem::path> macroPaths);
        }
    }
}
