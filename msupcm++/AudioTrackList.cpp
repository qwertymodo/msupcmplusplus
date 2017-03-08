#include "AudioTrackList.h"

using namespace msu;

AudioTrackList::AudioTrackList()
{

}


AudioTrackList::~AudioTrackList()
{

}


void AudioTrackList::render()
{
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
