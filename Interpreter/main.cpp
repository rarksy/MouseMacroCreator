#include <filesystem>
#include "Menu/Menu.h"
#include "Macro/Macro.h"

int main(int argc, char* argv[])
{
    if (argv[1] == nullptr)
    {
        std::cout << "MMC Interpreter: No File Found!";
        return 1;
    }
    
    MacroCore::RunMacro::Run(argv[1]);

    return 0;
}
