#include <filesystem>
#include "Menu/Menu.h"
#include "Macro/Macro.h"

//todo: implement valid argument check

int main(int argc, char* argv[])
{
    // Checking If The User Attached A File
    const bool hasFileAttached = argv[1] != nullptr;

    // They Did Not
    if (!hasFileAttached)
    {
        std::cout << "MMC Interpreter: No File Found!" << std::endl;
        _getch();
        return 1;
    }

    // They Did Attach A File! Lets Save It
    const std::filesystem::path macroPath = argv[1];

    // Lets Check If The Files Extension Is Correct
    const bool fileIsMacro = macroPath.extension() == ".mmc";

    // Its Not :(
    if (!fileIsMacro)
    {
        std::cout << "MMC Interpreter: Invalid File Type!" << std::endl;
        _getch();
        return 2;
    }

    // Running The Macro...
    // Since A Bool Returns 0 (false) On Failure And 1 (true) On Success
    // But 0 Generally Means Success In A Programs Lifetime We Use ! To Flip The Values
    return !MacroCore::RunMacro::Run(macroPath);
}
