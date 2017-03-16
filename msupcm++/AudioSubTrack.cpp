#include "AudioSubTrack.h"
#include "AudioSubChannel.h"
#include "GlobalConfig.h"
#include "SoxWrapper.h"

using namespace msu;

AudioSubTrack::AudioSubTrack() : AudioBase()
{
	m_sub_channels = 0;
	m_num_sub_channels = 0;
}


AudioSubTrack::AudioSubTrack(const char *in) : AudioBase(in)
{
	m_sub_channels = 0;
	m_num_sub_channels = 0;
}


AudioSubTrack::AudioSubTrack(const char *in, const char *out) : AudioBase(in, out)
{
	m_sub_channels = 0;
	m_num_sub_channels = 0;
}


AudioSubTrack::AudioSubTrack(const AudioSubTrack& a) : AudioBase(a)
{
	m_sub_channels = 0;
	m_num_sub_channels = 0;
	*this = a;
}


AudioSubTrack::~AudioSubTrack()
{
	clear();
}


AudioSubTrack& AudioSubTrack::operator=(const AudioSubTrack& a)
{
	clear();
	AudioBase::operator=(a);

	m_num_sub_channels = a.m_num_sub_channels;
	if (m_num_sub_channels)
	{
		AudioSubChannel* s = new AudioSubChannel[m_num_sub_channels];
		for (auto i = 0; i < m_num_sub_channels; ++i)
		{
			s[i] = dynamic_cast<AudioSubChannel*>(a.m_sub_channels)[i];
		}

		if (m_sub_channels)
		{
			delete[] m_sub_channels;
		}

		m_sub_channels = s;
	}
	else
	{
		m_sub_channels = 0;
	}

	return *this;
}


AudioSubTrack& AudioSubTrack::operator=(const AudioBase& a)
{
	this->~AudioSubTrack();
	AudioBase::operator=(a);
	return *this;
}


void AudioSubTrack::clear()
{
	if (m_sub_channels)
		delete[] dynamic_cast<AudioSubChannel*>(m_sub_channels);

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


void AudioSubTrack::addSubChannel(AudioBase* a)
{
	AudioSubChannel* s = new AudioSubChannel[m_num_sub_channels + 1];
	for (auto i = 0; i < m_num_sub_channels; ++i)
	{
		s[i] = dynamic_cast<AudioSubChannel*>(m_sub_channels)[i];
	}
	s[m_num_sub_channels++] = *dynamic_cast<AudioSubChannel*>(a);

	if (m_sub_channels)
	{
		delete[] dynamic_cast<AudioSubChannel*>(m_sub_channels);
	}

	m_sub_channels = s;
}


void AudioSubTrack::render()
{
	SoxWrapper* sox = SoxWrapperFactory::getInstance();

	if (m_num_sub_channels)
	{
		for (auto i = 0; i < m_num_sub_channels; ++i)
		{
			AudioSubChannel* p = &dynamic_cast<AudioSubChannel*>(m_sub_channels)[i];
			p->outFile() = m_outfile.substr(0, m_outfile.find_last_of(".")).append("_str").append(std::to_string(i)).append(".wav");
			p->render();
			if (!m_loop)
			{
				if (size_t loop = p->loop())
				{
					m_loop = ((loop - p->trimStart()) * 44100.0 / sox->inputRate() / sox->tempo());
				}
			}
		}

		if (sox->init(dynamic_cast<AudioSubChannel*>(m_sub_channels)[0].outFile(), m_outfile))
		{
			for (auto i = 1; i < m_num_sub_channels; ++i)
			{
				sox->addInput(dynamic_cast<AudioSubChannel*>(m_sub_channels)[i].outFile());
			}
			sox->setCombine(sox_mix);
			sox->crossFade(m_loop, m_trim_end, m_cross_fade);
			sox->trim(m_trim_start, m_trim_end);
			sox->fade(m_fade_in, m_fade_out);
			sox->pad(m_pad_start, m_pad_end);
			sox->tempo(m_tempo);
			sox->normalize(m_normalization);
			sox->setLoop(m_loop - m_trim_start);
			sox->finalize();
		}

		if (!config.keep_temps())
		{
			for (auto i = 0; i < m_num_sub_channels; ++i)
			{
				remove(dynamic_cast<AudioSubChannel*>(m_sub_channels)[i].outFile().c_str());
			}
		}
	}
	else
	{
		AudioBase::render();
	}
}
