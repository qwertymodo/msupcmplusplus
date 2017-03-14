#include "AudioTrackListBuilder.h"
#include "sox_main.h"

using namespace msu;

int main(int argc, char * argv[])
{
	switch (argc)
	{
	case 1:
		AudioTrackListBuilder("tracks.json").get().render();
		break;

	case 2:
		if (std::string(argv[1]).find_last_of(".") != std::string::npos  &&
			std::string(argv[1]).substr(std::string(argv[1]).find_last_of(".")).compare(".json") == 0)
		{
			AudioTrackListBuilder(argv[1]).get().render();
		}

		break;

	default:
		if (std::string(argv[1]).compare("-s") == 0)
		{
			delete argv[1];
			argv[1] = new char[4]{ 's','o','x','\0' };

			soxmain(--argc, ++argv);
		}
		exit(1);
	}
	
	return 0;
}
