#pragma once
#include <functional>
#include <string>
#include <vector>

enum MacroActionType
{
    MAT_KeyDown,
    MAT_KeyUp,
    MAT_MouseMove,
    MAT_MouseClick,

    MAT_Sleep
};

struct MacroAction
{
    MacroActionType actionType;
    std::string keyword;

    int intArgument;
    char charArgument;
    std::pair<int, int> pairArgument;
};

struct Macro
{
    std::vector<MacroAction> actions;
    std::string name;
    char id;
};

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

        namespace RunMacro
        {
            void Run(Macro macro);

            inline bool macroRunning = false;
            inline bool quitMacro = false;
        }
    }
}
