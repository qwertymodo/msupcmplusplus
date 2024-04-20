#include "AudioSubTrack.h"
#include "AudioSubChannel.h"
#include "GlobalConfig.h"
#include "SoxWrapper.h"
#include "utf8.h"

#include <fstream>

#ifdef WIN32
#define CONCAT(x, y) x##y
#define WSTR_L(x) CONCAT(L, x)
#else
#define WSTR_L(x) x
#define wchar_t char
#endif

using namespace msu;

AudioSubTrack::AudioSubTrack() : AudioBase()
{
	m_sub_channels = 0;
	m_num_sub_channels = 0;
}


AudioSubTrack::AudioSubTrack(std::fstring_t in) : AudioBase(in)
{
	m_sub_channels = 0;
	m_num_sub_channels = 0;
}


AudioSubTrack::AudioSubTrack(std::fstring_t in, std::fstring_t out) : AudioBase(in, out)
{
	m_sub_channels = 0;
	m_num_sub_channels = 0;
}


AudioSubTrack::AudioSubTrack(int argc, char** argv) : AudioBase(argc, argv),
    m_sub_channels(0), m_num_sub_channels(0)
{
    wchar_t** _argv = new wchar_t*[argc + 1];
    _argv[0] = WSTR_L("AudioSubTrack");
    int _argc = 1;

    for (auto i = 0; i < argc; ++i)
    {
        if (argv[i][0] == '(')
        {
            int s_argc = 0;
            char** s_argv = new char*[argc];
            s_argv[s_argc] = new char[strlen(argv[i])]{ 0 };

            if (argv[i][strlen(argv[i]) - 1] == ')')
            {
                strncpy(s_argv[s_argc++], argv[i] + 1, strlen(argv[i]) - 2);
            }

            else
            {
                strcpy(s_argv[s_argc++], argv[i++] + 1);
                for (int depth = 1; i < argc; ++i)
                {
                    s_argv[s_argc] = new char[strlen(argv[i])]{ 0 };

                    if (argv[i][strlen(argv[i]) - 1] == ')')
                        --depth;

                    if (depth == 0)
                    {
                        strncpy(s_argv[s_argc++], argv[i], strlen(argv[i]) - 1);
                        break;
                    }

                    else
                        strcpy(s_argv[s_argc++], argv[i]);

                    if (argv[i][0] == '(')
                        ++depth;
                }
            }

            addSubChannel(new AudioSubChannel(s_argc, s_argv));
        }
    }

    for (auto i = 1; i < _argc; ++i)
        delete _argv[i];
    delete[] _argv;
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
			if (p->inFile().empty())
				p->inFile() = inFile();

            // Read existing loop point from PCM inputs if one isn't explicitly specified
            if (p->inFile().substr(p->inFile().length() - 4).compare(WSTR_L(".pcm")) == 0 && p->loop() == 0)
            {
#ifdef WIN32
                std::ifstream infile(utf8_to_wstring.to_bytes(p->inFile()).c_str(), std::ios::in | std::ios::binary);
#else
                std::ifstream infile(p->inFile().c_str(), std::ios::in | std::ios::binary);
#endif
                if (infile.is_open())
                {
                    char signature[4];
                    infile.read(signature, 4);  // Verify file signature
                    if (strncmp(signature, "MSU1", 4) == 0)
                    {
                        infile.read((char*)&p->loop(), sizeof(p->loop()));
                    }
                }
                infile.close();
            }

			size_t loop = p->loop();
			if(p->trimStart() > p->loop())
				p->loop() = p->trimStart();

			if (m_loop && !p->loop())
				p->loop() = m_loop + p->trimStart();

#ifdef WIN32
			p->outFile() = m_outfile.substr(0, m_outfile.find_last_of(WSTR_L("."))).append(WSTR_L("_sch")).append(std::to_wstring(i)).append(WSTR_L(".wav"));
#else
			p->outFile() = m_outfile.substr(0, m_outfile.find_last_of(WSTR_L("."))).append(WSTR_L("_sch")).append(std::to_string(i)).append(WSTR_L(".wav"));
#endif
			p->render();
			if (!m_loop)
			{
				if (loop)
				{
					m_loop = ((loop - p->trimStart() + p->padStart()) * 44100.0 / sox->inputRate() / sox->tempo());
				}
			}
		}

		if (sox->init(dynamic_cast<AudioSubChannel*>(m_sub_channels)[0].outFile(), m_outfile))
		{
			if (m_loop && m_trim_start > m_loop)
			{
				m_start_offset = m_trim_start - m_loop;
				m_trim_start = m_loop;
			}

			for (auto i = 1; i < m_num_sub_channels; ++i)
			{
				sox->addInput(dynamic_cast<AudioSubChannel*>(m_sub_channels)[i].outFile());
			}
			sox->combine(sox_mix);
			if (m_compression)
				sox->compress();
			if (sox->crossFade(m_loop, m_trim_end, m_cross_fade))
				m_trim_end = 0;
			sox->trim(m_trim_start, m_trim_end);
			sox->normalize(m_normalization);
			sox->fade(m_fade_in, m_fade_out);
			sox->tempo(m_tempo);
			sox->loop(m_trim_start + m_start_offset, m_loop);
			sox->pad(m_pad_start, m_pad_end);
			sox->dither(m_dither_type);
			sox->finalize();
		}

		if (!config.keep_temps())
		{
			for (auto i = 0; i < m_num_sub_channels; ++i)
			{
#ifdef WIN32
				remove(utf8_to_wstring.to_bytes(dynamic_cast<AudioSubChannel*>(m_sub_channels)[i].outFile()).c_str());
#else
				remove(dynamic_cast<AudioSubChannel*>(m_sub_channels)[i].outFile().c_str());
#endif
			}
		}
	}
	else
	{
		AudioBase::render();
	}
}
