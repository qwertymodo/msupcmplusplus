#include "AudioTrackList.h"
#include <iostream>

using namespace msu;

AudioTrackList::AudioTrackList()
{

}


AudioTrackList::~AudioTrackList()
{

}


void AudioTrackList::render()
{
	if (GlobalConfig::verbosity() > 0)
	{
		std::cout << "MSU-1 Audio Conversion Tool" << std::endl;

		if (!GlobalConfig::game().empty())
			std::cout << "Game: " << GlobalConfig::game() << std::endl;

		if (!GlobalConfig::pack().empty())
			std::cout << "Pack: " << GlobalConfig::pack() << std::endl;

		if (!GlobalConfig::artist().empty())
			std::cout << "Artist: " << GlobalConfig::artist() << std::endl;

		if (!GlobalConfig::url().empty())
			std::cout << GlobalConfig::url() << std::endl;

		std::cout << std::endl;
	}

	for each (AudioTrack track in m_tracks)
	{
		track.render();
	}
}


const std::vector<AudioTrack>& AudioTrackList::tracks() const
{
	return m_tracks;
}


std::vector<AudioTrack>& AudioTrackList::tracks()
{
	return m_tracks;
}
