#include "debug.h"
#include <iostream>

int DebugStreamBuffer::sync() {
	auto s = str();
    auto cs = s.c_str();
    OutputDebugStringW(reinterpret_cast<LPCWSTR>(cs));
    // convert unicode to utf8
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &s[0], (int)s.size(), NULL, 0, NULL, NULL);
    std::string strTo( size_needed, 0 );
    WideCharToMultiByte(CP_UTF8, 0, &s[0], (int)s.size(), &strTo[0], size_needed, NULL, NULL);
    std::cout << strTo << std::flush;
	std::wstringbuf::str(L"");
	return 0;
}
