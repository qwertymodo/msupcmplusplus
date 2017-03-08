#include "AudioSubTrack.h"

using namespace msu;

AudioSubTrack::AudioSubTrack() :
	AudioBase()
{
	m_channels = 0;
	m_num_channels = 0;
}


AudioSubTrack::AudioSubTrack(const char *in) :
	AudioBase(in)
{
	m_channels = 0;
	m_num_channels = 0;
}


AudioSubTrack::AudioSubTrack(const char *in, const char *out) :
	AudioBase(in, out)
{
	m_channels = 0;
	m_num_channels = 0;
}


AudioSubTrack::~AudioSubTrack()
{
	if (m_channels)
		delete(m_channels);

	m_channels = 0;
	m_num_channels = 0;
}


AudioBase* AudioSubTrack::channels() const
{
	return m_channels;
}


int AudioSubTrack::numChannels() const
{
	return m_num_channels;
}


void AudioSubTrack::addChannel(const AudioBase& a)
{

}


void AudioSubTrack::removeChannel(int idx)
{

}


void AudioSubTrack::render()
{

}
