#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <filesystem>

enum MacroActionType
{
    MAT_KeyPress,
    MAT_KeyDown,
    MAT_KeyUp,

    MAT_MouseClick,
    MAT_MouseDown,
    MAT_MouseUp,
    MAT_SetMousePos,
    MAT_SetMousePosInterpolated,

    MAT_Sleep,

    MAT_Log,

    MAT_SetToggleKey,
    MAT_SetTerminateKey,
    MAT_SetToggleType,
};

enum MacroToggleType
{
    MTT_Toggle,
    MTT_HoldOn,
    MTT_HoldOff,
    MTT_RunOnce
};

struct MacroAction
{
    MacroActionType actionType;
    std::string keyword;
    std::vector<std::string> parameters;

    int intArgument = 0;
    char charArgument;
    std::pair<int, int> pairArgument;
    std::string stringArgument;

    bool keyPressToUpper = false;
};

struct Macro
{
    std::vector<MacroAction> actions;
    std::string name;

    int toggleKey = VK_F8;
    int terminateKey = VK_F9;

    MacroToggleType toggleType = MTT_Toggle;
    
    bool hasError = false;
};

namespace MacroCore
{
    inline std::vector<std::pair<std::string, MacroActionType>> validKeywords{

        // Function Keywords
        {"SetMousePos", MAT_SetMousePos},
        {"SetMousePosInterpolated", MAT_SetMousePosInterpolated},

        {"Sleep", MAT_Sleep},
        {"Log", MAT_Log},

        {"MouseClick", MAT_MouseClick},
        {"MouseDown", MAT_MouseDown},
        {"MouseUp", MAT_MouseUp},

        {"KeyPress", MAT_KeyPress},
        {"KeyDown", MAT_KeyDown},
        {"KeyUp", MAT_KeyUp},

        // Macro State Keywords

        {"SetToggleKey", MAT_SetToggleKey},
        {"SetTerminateKey", MAT_SetTerminateKey},

        {"SetToggleType", MAT_SetToggleType},
    };

    inline std::vector<std::pair<std::string, MacroToggleType>> validToggleTypes {

        {"Toggle", MTT_Toggle},
        {"HoldOn", MTT_HoldOn},
        {"HoldOff", MTT_HoldOff},
        {"RunOnce", MTT_RunOnce},
    };

    inline std::vector<std::pair<std::string, std::pair<int, int>>> validMouseButtons {

        {"Left", {MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP}},
        {"Right", {MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP}},
        {"Middle", {MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP}},
        {"Thumb1", {MOUSEEVENTF_XDOWN, MOUSEEVENTF_XUP}},
        {"Thumb2", {MOUSEEVENTF_XDOWN, MOUSEEVENTF_XUP}}

    };
    
    inline std::vector<std::pair<std::string, int>> validKeys{

        // Key Documentation:
        // The Keys Quoted In " " Are The Keys Used In Your Macro File
        // Example: KeyDown Shift
        // Example: KeyDown F1
        // Example: KeyDown A
        // Example: KeyDown 1
        // Example: KeyDown .

        // Modifier Keys
        {"Shift", VK_SHIFT},
        {"Control", VK_CONTROL},
        {"Windows", VK_LWIN},
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
    
    template<typename T>
    bool EnsureValidKeyword(const std::string& line, std::string& keyword, std::vector<std::pair<std::string, T>> vec);
    MacroAction ProcessAction(std::istringstream& iss, const std::string& line, Macro& macro);
    void ExecuteAction(const MacroAction& action);

    namespace ThreadFlow
    {
        namespace Sleep
        {
            void Process(std::istringstream& iss, MacroAction& action);
            void Execute(const MacroAction& action);   
        }

        namespace Log
        {
            void Process(std::istringstream& iss, MacroAction& action);
            void Execute(const MacroAction& action);   
        }
    }

    namespace MouseInput
    {
        namespace SetMousePos
        {
            void Process(std::istringstream& iss, MacroAction& action);
            void Execute(const MacroAction& action);   
        }

        namespace SetMousePosInterpolated
        {
            void Process(std::istringstream& iss, MacroAction& action);
            void Execute(const MacroAction& action);   
        }

        namespace MouseDownUp
        {
            void Process(std::istringstream& iss, MacroAction& action);
            void Execute(const MacroAction& action); 
        }

        namespace MouseClick
        {
            void Process(std::istringstream& iss, MacroAction& action);
            void Execute(const MacroAction& action); 
        }
    }

    namespace KeyboardInput
    {
        namespace KeyDownUp
        {
            void Process(std::istringstream& iss, MacroAction& action);
            void Execute(const MacroAction& action);   
        }

        namespace KeyPress
        {
            void Process(std::istringstream& iss, MacroAction& action);
            void Execute(const MacroAction& action);   
        }
    }

    namespace MacroState
    {
        namespace SetStateKey
        {
            void Process(std::istringstream& iss, int& stateKey);
        }

        namespace SetToggleType
        {
            void Process(std::istringstream&iss, Macro& macro);
        }
    }

    namespace RunMacro
    {
        void Run(const std::filesystem::path& path);

        inline bool macroRunning = false;
        inline bool quitMacro = false;
    }
}
