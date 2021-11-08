#pragma once
#include <iostream>

#ifdef EXE
#define API __declspec(dllimport)
#else
#define API __declspec(dllexport)
#endif

#define GET_WIN32_LAST_ERROR                                                                                                                                                  \
    {                                                                                                                                                                               \
        DWORD err = GetLastError();                                                                                                                                                 \
        LPSTR buffer;                                                                                                                                                               \
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, err, 0, reinterpret_cast<LPSTR>(&buffer), 0, nullptr); \
        std::cout << buffer;                          \
    }