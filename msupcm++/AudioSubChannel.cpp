#include "AudioSubChannel.h"

using namespace msu;

AudioSubChannel::AudioSubChannel() :
	AudioBase()
{
	m_sub_tracks = 0;
	m_num_sub_tracks = 0;
}


AudioSubChannel::AudioSubChannel(const char *in) :
	AudioBase(in)
{
	m_sub_tracks = 0;
	m_num_sub_tracks = 0;
}


AudioSubChannel::AudioSubChannel(const char *in, const char *out) :
	AudioBase(in, out)
{
	m_sub_tracks = 0;
	m_num_sub_tracks = 0;
}


AudioSubChannel::~AudioSubChannel()
{
	if (m_sub_tracks)
		delete(m_sub_tracks);

	m_sub_tracks = 0;
	m_num_sub_tracks = 0;
}


AudioBase* AudioSubChannel::subtracks() const
{
	return m_sub_tracks;
}


int AudioSubChannel::numSubtracks() const
{
	return m_num_sub_tracks;
}


void AudioSubChannel::addSubtrack(const AudioBase& a)
{

}


void AudioSubChannel::removeSubtrack(int idx)
{

}


void AudioSubChannel::render()
{

}
