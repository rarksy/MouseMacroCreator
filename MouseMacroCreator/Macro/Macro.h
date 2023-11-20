#pragma once
#include <vector>
#include <string>
#include <variant>
#include <Windows.h>
#include <filesystem>

enum MacroActionType
{
    MAT_KeyDown,
    MAT_KeyUp,

    MAT_MouseDown,
    MAT_MouseUp,

    MAT_MouseMove,

    MAT_Sleep,

    MAT_Parameter
};

struct MacroAction
{
    MacroActionType actionType;
    std::string keyword;

    int intArgument;
    char charArgument;
    std::pair<int, int> pairArgument;
    std::string stringArgument;
};

struct Macro
{
    std::vector<MacroAction> actions;
    std::string name;

    bool hasError = false;
};

namespace MacroCore
{
    
    bool IsValidKeyword(std::string line, std::string keyword);

    MacroAction ProcessAction(std::istringstream& iss, std::string line);

    void ExecuteAction(const MacroAction& action);

    namespace RunMacro
    {
        void Run(const std::filesystem::path& path);

        inline bool macroRunning = false;
        inline bool quitMacro = false;
    }


    inline std::vector<Macro> allMacros;
    
    inline std::vector<std::pair<std::string, MacroActionType>> validKeywords{

        // Function Keywords
        {"SetMousePos", MAT_MouseMove},

        {"Sleep", MAT_Sleep},

        {"MouseDown", MAT_MouseDown},
        {"MouseUp", MAT_MouseUp},

        {"KeyDown", MAT_KeyDown},
        {"KeyUp", MAT_KeyUp},

        {"Left", MAT_Parameter},
        {"Right", MAT_Parameter},
        {"Middle", MAT_Parameter},
        
        
    };


    inline std::vector<std::pair<std::string, int>> validKeys{

        // Key Documentation:
        // The Keys Quoted In "" Are The Keys Used In Your Macro File
        // Example: KeyDown Shift
        // Example: KeyDown Control
        // Example: KeyDown F1
        // Example: KeyDown A
        // Example: KeyDown 1
        // Example: KeyDown .

        // Letters
        {"a", 'a'},
        {"b", 'b'},
        {"c", 'c'},
        {"d", 'd'},
        {"e", 'e'},
        {"f", 'f'},
        {"g", 'g'},
        {"h", 'h'},
        {"i", 'i'},
        {"j", 'j'},
        {"k", 'k'},
        {"l", 'l'},
        {"m", 'm'},
        {"n", 'n'},
        {"o", 'o'},
        {"p", 'p'},
        {"q", 'q'},
        {"r", 'r'},
        {"s", 's'},
        {"t", 't'},
        {"u", 'u'},
        {"v", 'v'},
        {"w", 'w'},
        {"x", 'x'},
        {"y", 'y'},
        {"z", 'z'},

        
        // Numbers
        {"0", '0'},
        {"1", '1'},
        {"2", '2'},
        {"3", '3'},
        {"4", '4'},
        {"5", '5'},
        {"6", '6'},
        {"7", '7'},
        {"8", '8'},
        {"9", '9'},

        //Modifier Keys
        {"Shift", VK_SHIFT},
        {"Control", VK_CONTROL},
        {"Alt", VK_MENU},
        {"Enter", VK_RETURN},
        {"Space", VK_SPACE},
        {"Tab", VK_TAB},
        {"Escape", VK_ESCAPE},
        {"Backspace", VK_BACK},
        {"Delete", VK_DELETE},
        {"Up", VK_UP},
        {"Down", VK_DOWN},
        {"Left", VK_LEFT},
        {"Right", VK_RIGHT},
        {"Home", VK_HOME},
        {"End", VK_END},
        {"Page Up", VK_PRIOR},
        {"Page Down", VK_NEXT},
        {"F1", VK_F1},
        {"F2", VK_F2},
        {"F3", VK_F3},
        {"F4", VK_F4},
        {"F5", VK_F5},
        {"F6", VK_F6},
        {"F7", VK_F7},
        {"F8", VK_F8},
        {"F9", VK_F9},
        {"F10", VK_F10},
        {"F11", VK_F11},
        {"F12", VK_F12}
    };
}



