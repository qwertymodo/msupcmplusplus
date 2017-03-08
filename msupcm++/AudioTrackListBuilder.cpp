#include "AudioTrackListBuilder.h"
#include "TrackParser.hpp"
#include <fstream>
#include <iostream>

using namespace msu;

AudioTrackListBuilder::AudioTrackListBuilder(std::string config)
{
	std::ifstream ifs(config);
	if (ifs.is_open())
	{
		json j;
		ifs >> j;

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
