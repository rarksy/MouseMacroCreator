#pragma once
#include <filesystem>

namespace GUI
{
    
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
