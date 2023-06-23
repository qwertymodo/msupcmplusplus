#include <codecvt>
#include <locale>
#include <string>

extern std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_to_wstring;

#ifdef _WIN32
char *utf16_to_utf8(const wchar_t *input);
#else
#define utf16_to_utf8(input)
#endif