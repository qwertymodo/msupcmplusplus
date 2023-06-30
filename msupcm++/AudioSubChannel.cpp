#include "AudioSubChannel.h"
#include "AudioSubTrack.h"
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

AudioSubChannel::AudioSubChannel() :
	AudioBase()
{
	m_sub_tracks = 0;
	m_num_sub_tracks = 0;
}


AudioSubChannel::AudioSubChannel(std::fstring_t in) : AudioBase(in)
{
	m_sub_tracks = 0;
	m_num_sub_tracks = 0;
}


AudioSubChannel::AudioSubChannel(std::fstring_t in, std::fstring_t out) : AudioBase(in, out)
{
	m_sub_tracks = 0;
	m_num_sub_tracks = 0;
}


AudioSubChannel::AudioSubChannel(int argc, char** argv) : AudioBase(argc, argv),
    m_sub_tracks(0), m_num_sub_tracks(0)
{
    wchar_t** _argv = new wchar_t*[argc + 1];
    _argv[0] = WSTR_L("AudioSubChannel");
    int _argc = 1;

    for (auto i = 0; i < argc; ++i)
    {
        if (argv[i][0] == '[')
        {
            int s_argc = 0;
            char** s_argv = new char*[argc];
            s_argv[s_argc] = new char[strlen(argv[i])]{ 0 };

            if (argv[i][strlen(argv[i]) - 1] == ']')
            {
                strncpy(s_argv[s_argc++], argv[i] + 1, strlen(argv[i]) - 2);
            }

            else
            {
                strcpy(s_argv[s_argc++], argv[i++] + 1);
                for (int depth = 1; i < argc; ++i)
                {
                    s_argv[s_argc] = new char[strlen(argv[i])]{ 0 };

                    if (argv[i][strlen(argv[i]) - 1] == ']')
                        --depth;

                    if (depth == 0)
                    {
                        strncpy(s_argv[s_argc++], argv[i], strlen(argv[i]) - 1);
                        break;
                    }

                    else
                        strcpy(s_argv[s_argc++], argv[i]);

                    if (argv[i][0] == '[')
                        ++depth;
                }
            }

			addSubTrack(new AudioSubTrack(s_argc, s_argv));
        }
    }

    for (auto i = 1; i < _argc; ++i)
        delete _argv[i];
    delete[] _argv;
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
			if (p->trimStart() > p->loop())
				p->loop() = p->trimStart();

#ifdef WIN32
			p->outFile() = m_outfile.substr(0, m_outfile.find_last_of(WSTR_L("."))).append(WSTR_L("_str")).append(std::to_wstring(i)).append(WSTR_L(".wav"));
#else
			p->outFile() = m_outfile.substr(0, m_outfile.find_last_of(WSTR_L("."))).append(WSTR_L("_str")).append(std::to_string(i)).append(WSTR_L(".wav"));
#endif
			p->render();
			if (!m_loop)
			{
				if (loop)
				{
					m_loop = sub_loop + ((loop - p->trimStart() + p->padStart()) * 44100.0 / sox->inputRate() / sox->tempo());
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
			if (m_compression)
				sox->compress();
			if (sox->crossFade(m_loop, m_trim_end, m_cross_fade))
				m_trim_end = 0;
			sox->trim(m_trim_start, m_trim_end);
			sox->normalize(m_normalization);
			sox->fade(m_fade_in, m_fade_out);
			sox->tempo(m_tempo);
			sox->pad(m_pad_start, m_pad_end);
			sox->loop(m_trim_start + m_start_offset, m_loop + m_pad_start);
			sox->dither(m_dither_type);
			sox->finalize();
		}

		for (auto i = 0; i < m_num_sub_tracks; ++i)
		{
			if (!config.keep_temps())
#ifdef WIN32
				remove(utf8_to_wstring.to_bytes(dynamic_cast<AudioSubTrack*>(m_sub_tracks)[i].outFile()).c_str());
#else
				remove(dynamic_cast<AudioSubTrack*>(m_sub_tracks)[i].outFile().c_str());
#endif
		}
	}
	else
	{
		AudioBase::render();
	}
}
