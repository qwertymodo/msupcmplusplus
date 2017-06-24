#include <codecvt>
#include <iostream>
#include <string>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "AudioTrackListBuilder.h"
#include "sox_main.h"

#define VERSION_NUMBER 0.5

using namespace msu;

void usage()
{
	std::wcout << L"msupcm v" << VERSION_NUMBER << std::endl << std::endl;
	std::wcout << L"Usage:" << std::endl;
	std::wcout << L"msupcm [tracks.json]" << std::endl;
	std::wcout << L"msupcm -s [sox args]" << std::endl;
	std::wcout << L"msupcm -v" << std::endl;
	std::wcout << L"msupcm ?" << std::endl;
}

static char *utf16_to_utf8(const wchar_t *input)
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

int main(int argc, char * argv[])
{
	int wargc;
	char** wargv;
	int exit_code;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> wchar_to_utf8;

	lsx_init_console();
	lsx_init_commandline_arguments(&wargc, &wargv);

	switch (argc)
	{
	case 1:
		AudioTrackListBuilder(L"tracks.json").get().render();
		exit_code = 0;
		break;

	case 2:
		if (std::wstring(wchar_to_utf8.from_bytes(wargv[1])).compare(L"-v") == 0)
		{
			std::wcout << L"msupcm v" << VERSION_NUMBER << std::endl;
			exit_code = 0;
			break;
		}

		else if (std::wstring(wchar_to_utf8.from_bytes(wargv[1])).compare(L"?") == 0)
		{
			usage();
			exit_code = 0;
			break;
		}

		else if (std::wstring(wchar_to_utf8.from_bytes(wargv[1])).find_last_of(L".") != std::wstring::npos  &&
			std::wstring(wchar_to_utf8.from_bytes(wargv[1])).substr( \
				std::wstring(wchar_to_utf8.from_bytes(wargv[1])).find_last_of(L".")).compare(L".json") == 0)
		{
			AudioTrackListBuilder(wchar_to_utf8.from_bytes(wargv[1])).get().render();
		}

		else
		{
			usage();
			exit_code = 1;
			break;
		}

		break;

	default:
		if (std::string(argv[1]).compare("-s") == 0)
		{
			wargv[1] = utf16_to_utf8(L"sox");
			--wargc;

			exit_code = soxmain(wargc, wargv + 1);
			break;
		}

		usage();
		exit_code = 1;
		break;
	}
	
	lsx_uninit_console();
	lsx_free_commandline_arguments(&wargc, &wargv);

	return exit_code;
}
