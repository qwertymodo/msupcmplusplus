#include "AudioTrack.h"
#include <assert.h>

using namespace msu;

AudioTrack::AudioTrack():
	AudioBase()
{
	int x = 0;
}


AudioTrack::AudioTrack(const char *in):
	AudioBase(in)
{

}


AudioTrack::AudioTrack(const char *in, const char *out):
	AudioBase(in, out)
{

}


AudioTrack::~AudioTrack()
{
	
}


void AudioTrack::render()
{
	assert(m_subtracks == 0 || m_channels == 0);
	AudioBase::render();
}


const std::string& AudioTrack::title() const
{
	return m_title;
}


std::string& AudioTrack::title()
{
	return m_title;
}
