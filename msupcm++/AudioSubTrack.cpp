#include "AudioSubTrack.h"

using namespace msu;

AudioSubTrack::AudioSubTrack() :
	AudioBase()
{
	m_sub_channels = 0;
	m_num_sub_channels = 0;
}


AudioSubTrack::AudioSubTrack(const char *in) :
	AudioBase(in)
{
	m_sub_channels = 0;
	m_num_sub_channels = 0;
}


AudioSubTrack::AudioSubTrack(const char *in, const char *out) :
	AudioBase(in, out)
{
	m_sub_channels = 0;
	m_num_sub_channels = 0;
}


AudioSubTrack::~AudioSubTrack()
{
	if (m_sub_channels)
		delete(m_sub_channels);

	m_sub_channels = 0;
	m_num_sub_channels = 0;
}


AudioBase* AudioSubTrack::subChannels() const
{
	return m_sub_channels;
}


int AudioSubTrack::numSubChannels() const
{
	return m_num_sub_channels;
}


void AudioSubTrack::addSubChannel(const AudioBase& a)
{

}


void AudioSubTrack::removeSubChannel(int idx)
{

}


void AudioSubTrack::render()
{

}
