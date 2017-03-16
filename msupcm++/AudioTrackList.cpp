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
	if (config.verbosity() > 0)
	{
		std::cout << "MSU-1 Audio Conversion Tool" << std::endl;

		if (!config.game().empty())
			std::cout << "Game: " << config.game() << std::endl;

		if (!config.pack().empty())
			std::cout << "Pack: " << config.pack() << std::endl;

		if (!config.artist().empty())
			std::cout << "Artist: " << config.artist() << std::endl;

		if (!config.url().empty())
			std::cout << config.url() << std::endl;

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
