#pragma once
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

        namespace ViewSavedMacros
        {
            void Run();
        }

        namespace RunMacro
        {
            void Run(Macro macro);

            inline bool macroRunning = false;
            inline bool quitMacro = false;
        }
    }
}
