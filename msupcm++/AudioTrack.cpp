#include "AudioTrack.h"
#include "GlobalConfig.h"
#include <assert.h>
#include <iostream>

using namespace msu;

AudioTrack::AudioTrack(): AudioBase()
{
	m_track_number = 0;
	m_title = "";
}


AudioTrack::AudioTrack(const char *in): AudioBase(in)
{
	m_track_number = 0;
	m_title = "";
}


AudioTrack::AudioTrack(const char *in, const char *out): AudioBase(in, out)
{
	m_track_number = 0;
	m_title = "";
}


AudioTrack::~AudioTrack()
{
	
}


void AudioTrack::render()
{
	if (GlobalConfig::verbosity() > 0)
	{
		std::cout << "Track " << m_track_number;

		if (!m_title.empty())
			std::cout << ": " << m_title;

		std::cout << std::endl;
	}
	assert(m_subtracks == 0 || m_channels == 0);
	AudioBase::render();
}


int AudioTrack::trackNumber() const
{
	return m_track_number;
}


int& AudioTrack::trackNumber()
{
	return m_track_number;
}


const std::string& AudioTrack::title() const
{
	return m_title;
}


std::string& AudioTrack::title()
{
	return m_title;
}
