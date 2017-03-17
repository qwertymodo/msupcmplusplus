#include <iostream>
#include "AudioTrackListBuilder.h"
#include "sox_main.h"

#define VERSION_NUMBER 0.2

using namespace msu;

void usage()
{
	std::cout << "msupcm v" << VERSION_NUMBER << std::endl << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "msupcm [tracks.json]" << std::endl;
	std::cout << "msupcm -s [sox args]" << std::endl;
	std::cout << "msupcm -v" << std::endl;
	std::cout << "msupcm ?" << std::endl;
}

int main(int argc, char * argv[])
{
	switch (argc)
	{
	case 1:
		AudioTrackListBuilder("tracks.json").get().render();
		break;

	case 2:
		if (std::string(argv[1]).compare("-v") == 0)
		{
			std::cout << "msupcm v" << VERSION_NUMBER << std::endl;
			break;
		}

		else if (std::string(argv[1]).compare("?") == 0)
		{
			usage();
			break;
		}

		else if (std::string(argv[1]).find_last_of(".") != std::string::npos  &&
			std::string(argv[1]).substr(std::string(argv[1]).find_last_of(".")).compare(".json") == 0)
		{
			AudioTrackListBuilder(argv[1]).get().render();
		}

		else
		{
			usage();
			exit(1);
		}

		break;

	default:
		if (std::string(argv[1]).compare("-s") == 0)
		{
			argv[1] = new char[4]{ 's','o','x','\0' };

			exit(soxmain(--argc, ++argv));
		}

		usage();
		exit(1);
	}
	
	return 0;
}
