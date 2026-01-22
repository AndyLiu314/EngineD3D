#pragma once
#include <string>

std::wstring ToWide(const std::string& str);
std::string ToNarrow(const std::wstring& wstr);
