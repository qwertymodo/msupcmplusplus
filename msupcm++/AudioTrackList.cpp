#include "AudioTrackList.h"
#include "GlobalConfig.h"
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
		std::wcout << L"MSU-1 Audio Conversion Tool" << std::endl;

		if (!config.game().empty())
			std::wcout << L"Game: " << config.game() << std::endl;

		if (!config.pack().empty())
			std::wcout << L"Pack: " << config.pack() << std::endl;

		if (!config.artist().empty())
			std::wcout << L"Artist: " << config.artist() << std::endl;

		if (!config.url().empty())
			std::wcout << config.url() << std::endl;

		std::wcout << std::endl;
	}

	for (AudioTrack track : m_tracks)
	{
		if ((track.trackNumber() >= config.first_track()) &&
			(config.last_track() < 0 || track.trackNumber() <= config.last_track()))
		{
			track.render();
		}
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
