#include "utf8.h"

std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_to_wstring;

#ifdef _WIN32
#include <windows.h>

char *utf16_to_utf8(const wchar_t *input)
{
    char *Buffer;
    int BuffSize = 0, Result = 0;

    BuffSize = WideCharToMultiByte(CP_UTF8, 0, input, -1, NULL, 0, NULL, NULL);
    Buffer = (char*)malloc(sizeof(char) * BuffSize);
    if (Buffer)
    {
        Result = WideCharToMultiByte(CP_UTF8, 0, input, -1, Buffer, BuffSize, NULL, NULL);
    }

    return ((Result > 0) && (Result <= BuffSize)) ? Buffer : NULL;
}
#endif
