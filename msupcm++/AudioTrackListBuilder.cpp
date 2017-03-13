#include "AudioTrackListBuilder.h"
#include "GlobalConfig.h"
#include "TrackParser.hpp"
#include <exception>
#include <fstream>
#include <iostream>

using namespace msu;

AudioTrackListBuilder::AudioTrackListBuilder(std::string config)
{
	std::ifstream ifs(config);
	if (ifs.is_open())
	{
		json j;
		try
		{
			ifs >> j;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			exit(1);
		}

		m_list = j;
	}
}


const AudioTrackList& AudioTrackListBuilder::get() const
{
	return m_list;
}


AudioTrackList& AudioTrackListBuilder::get()
{
	return m_list;
}
