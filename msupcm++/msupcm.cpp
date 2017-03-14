#include "AudioTrackListBuilder.h"

using namespace msu;

int main(int argc, char * argv[])
{

	std::string conf;

	if (argc > 1)
	{
		conf = argv[1];
	}
	else
	{
		conf = "tracks.json";
	}	

	AudioTrackListBuilder(conf).get().render();

	
	return 0;
}
