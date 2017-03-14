#include "AudioTrack.h"
#include "GlobalConfig.h"
#include <assert.h>
#include <iostream>
#include <vector>

using namespace msu;

AudioTrack::AudioTrack(): AudioBase()
{
	m_track_number = 0;
	m_title.clear();
}


AudioTrack::AudioTrack(const char *in): AudioBase(in)
{
	m_track_number = 0;
	m_title.clear();
}


AudioTrack::AudioTrack(const char *in, const char *out): AudioBase(in, out)
{
	m_track_number = 0;
	m_title.clear();
}


AudioTrack::AudioTrack(const AudioTrack& a)
{
	m_track_number = 0;
	m_title.clear();
	*this = a;
}


AudioTrack::~AudioTrack()
{

}


AudioTrack& AudioTrack::operator=(const AudioTrack& a)
{
	clear();
	if (a.m_num_sub_channels)
	{
		AudioSubTrack::operator=(a);
	}
	else if (a.m_num_sub_tracks)
	{
		AudioSubChannel::operator=(a);
	}
	else
	{
		AudioBase::operator=(a);
	}
	

	m_track_number = a.m_track_number;
	m_title = a.m_title;

	return *this;
}


AudioTrack& AudioTrack::operator=(const AudioSubChannel& a)
{
	clear();
	AudioSubChannel::operator=(a);

	return *this;
}


AudioTrack& AudioTrack::operator=(const AudioSubTrack& a)
{
	clear();
	AudioSubTrack::operator=(a);

	return *this;
}


AudioTrack& AudioTrack::operator=(const AudioBase& a)
{
	AudioBase::clear();
	AudioBase::operator=(a);

	return *this;
}


void AudioTrack::clear()
{
	AudioSubChannel::clear();
	AudioSubTrack::clear();

	m_track_number = 0;
	m_title.clear();
}


void AudioTrack::render()
{
	if (GlobalConfig::verbosity() > 0 && (!inFile().empty() || m_num_sub_channels > 0 || m_num_sub_tracks > 0))
	{
		std::cout << "Track " << m_track_number;

		if (!m_title.empty())
			std::cout << ": " << m_title;

		std::cout << std::endl;
	}

	assert(m_num_sub_tracks == 0 || m_num_sub_channels == 0);
	if (m_num_sub_tracks)
	{
		AudioSubChannel::render();
	}
	else if (m_num_sub_channels)
	{
		AudioSubTrack::render();
	}
	else
	{
		AudioBase::render();
	}
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
