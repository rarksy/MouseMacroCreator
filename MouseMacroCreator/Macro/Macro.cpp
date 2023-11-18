#include "Macro.h"

bool MacroCore::IsValidKeyword(std::string line, std::string keyword)
{
    return
        line.find(keyword) != std::string::npos &&
        std::find(
            validKeywords.begin(),
            validKeywords.end(), keyword
        ) != validKeywords.end();
}