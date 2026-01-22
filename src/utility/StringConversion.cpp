#include "utility/StringConversion.h"
#include "win/CustomWin.h"

std::wstring ToWide(const std::string& str)
{
    if (str.empty())
        return {};

    int size = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.data(),
        (int)str.size(),
        nullptr,
        0
    );

    std::wstring result(size, 0);
    MultiByteToWideChar(
        CP_UTF8,
        0,
        str.data(),
        (int)str.size(),
        result.data(),
        size
    );

    return result;
}
