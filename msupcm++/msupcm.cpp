#include "sox.h"
#include "AudioTrackListBuilder.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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

	/* All libSoX applications must start by initialising the SoX library */
	assert(sox_init() == SOX_SUCCESS);

	AudioTrackListBuilder(conf).get().render();

	sox_quit();
	return 0;
}
