#include "AudioSubChannel.h"
#include "AudioSubTrack.h"
#include "GlobalConfig.h"
#include "SoxWrapper.h"

using namespace msu;

AudioSubChannel::AudioSubChannel() :
	AudioBase()
{
	m_sub_tracks = 0;
	m_num_sub_tracks = 0;
}


AudioSubChannel::AudioSubChannel(std::wstring in) : AudioBase(in)
{
	m_sub_tracks = 0;
	m_num_sub_tracks = 0;
}


AudioSubChannel::AudioSubChannel(std::wstring in, std::wstring out) : AudioBase(in, out)
{
	m_sub_tracks = 0;
	m_num_sub_tracks = 0;
}


AudioSubChannel::AudioSubChannel(const AudioSubChannel& a) : AudioBase(a)
{
	m_sub_tracks = 0;
	m_num_sub_tracks = 0;
	*this = a;
}


AudioSubChannel::~AudioSubChannel()
{
	clear();
}


void AudioSubChannel::clear()
{
	if (m_sub_tracks)
		delete[] dynamic_cast<AudioSubTrack*>(m_sub_tracks);

	m_sub_tracks = 0;
	m_num_sub_tracks = 0;
}


AudioSubChannel& AudioSubChannel::operator=(const AudioSubChannel& a)
{
	clear();
	AudioBase::operator=(a);

	m_num_sub_tracks = a.m_num_sub_tracks;
	if (m_num_sub_tracks)
	{
		AudioSubTrack* s = new AudioSubTrack[m_num_sub_tracks];
		for (auto i = 0; i < m_num_sub_tracks; ++i)
		{
			s[i] = dynamic_cast<AudioSubTrack*>(a.m_sub_tracks)[i];
		}

		if (m_sub_tracks)
		{
			delete[] m_sub_tracks;
		}

		m_sub_tracks = s;
	}
	else
	{
		m_sub_tracks = 0;
	}

	return *this;
}


AudioSubChannel& AudioSubChannel::operator=(const AudioBase& a)
{
	this->~AudioSubChannel();
	AudioBase::operator=(a);
	return *this;
}


AudioBase* AudioSubChannel::subTracks() const
{
	return m_sub_tracks;
}


int AudioSubChannel::numSubTracks() const
{
	return m_num_sub_tracks;
}


void AudioSubChannel::addSubTrack(AudioBase* a)
{
	AudioSubTrack* s = new AudioSubTrack[m_num_sub_tracks + 1];
	for (auto i = 0; i < m_num_sub_tracks; ++i)
	{
		s[i] = dynamic_cast<AudioSubTrack*>(m_sub_tracks)[i];
	}
	s[m_num_sub_tracks++] = *dynamic_cast<AudioSubTrack*>(a);

	if (m_sub_tracks)
	{
		delete[] dynamic_cast<AudioSubTrack*>(m_sub_tracks);
	}

	m_sub_tracks = s;
}


void AudioSubChannel::render()
{
	SoxWrapper* sox = SoxWrapperFactory::getInstance();

	if (m_num_sub_tracks)
	{
		size_t sub_loop = 0;
		for (auto i = 0; i < m_num_sub_tracks; ++i)
		{
			AudioSubTrack* p = &dynamic_cast<AudioSubTrack*>(m_sub_tracks)[i];
			size_t loop = p->loop();
			if (p->trimStart() > p->loop())
				p->loop() = p->trimStart();

			p->outFile() = m_outfile.substr(0, m_outfile.find_last_of(L".")).append(L"_str").append(std::to_wstring(i)).append(L".wav");
			p->render();
			if (!m_loop)
			{
				if (loop)
				{
					m_loop = sub_loop + ((loop - p->trimStart()) * 44100.0 / sox->inputRate() / sox->tempo());
				}
				else
				{
					sub_loop += sox->length();
				}
			}
		}

		if (sox->init(dynamic_cast<AudioSubTrack*>(m_sub_tracks)[0].outFile(), m_outfile))
		{
			if (m_loop && m_trim_start > m_loop)
			{
				m_start_offset = m_trim_start - m_loop;
				m_trim_start = m_loop;
			}

			for (auto i = 1; i < m_num_sub_tracks; ++i)
			{
				sox->addInput(dynamic_cast<AudioSubTrack*>(m_sub_tracks)[i].outFile());
			}
			sox->combine(sox_concatenate);
			if (m_compress)
				sox->compress();
			if (sox->crossFade(m_loop, m_trim_end, m_cross_fade))
				m_trim_end = 0;
			sox->trim(m_trim_start, m_trim_end);
			sox->normalize(m_normalization);
			sox->fade(m_fade_in, m_fade_out);
			sox->pad(m_pad_start, m_pad_end);
			sox->tempo(m_tempo);
			sox->loop(m_trim_start + m_start_offset, m_loop);
			sox->dither(m_dither_type);
			sox->finalize();
		}

		for (auto i = 0; i < m_num_sub_tracks; ++i)
		{
			if (!config.keep_temps())
				_wremove(dynamic_cast<AudioSubTrack*>(m_sub_tracks)[i].outFile().c_str());
		}
	}
	else
	{
		AudioBase::render();
	}
}
