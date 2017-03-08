#include "AudioChannel.h"

using namespace msu;

AudioChannel::AudioChannel() :
	AudioBase()
{
	m_subtracks = 0;
	m_num_subtracks = 0;
}


AudioChannel::AudioChannel(const char *in) :
	AudioBase(in)
{
	m_subtracks = 0;
	m_num_subtracks = 0;
}


AudioChannel::AudioChannel(const char *in, const char *out) :
	AudioBase(in, out)
{
	m_subtracks = 0;
	m_num_subtracks = 0;
}


AudioChannel::~AudioChannel()
{
	if (m_subtracks)
		delete(m_subtracks);

	m_subtracks = 0;
	m_num_subtracks = 0;
}


AudioBase* AudioChannel::subtracks() const
{
	return m_subtracks;
}


int AudioChannel::numSubtracks() const
{
	return m_num_subtracks;
}


void AudioChannel::addSubtrack(const AudioBase& a)
{

}


void AudioChannel::removeSubtrack(int idx)
{

}


void AudioChannel::render()
{

}
