#include "AudioBase.h"
#include "GlobalConfig.h"
#include "SoxWrapper.h"
#include "utf8.h"
#include <iostream>
#include <fstream>

#ifdef _WIN32
#include "getopt.h"
#define CONCAT(x, y) x##y
#define WSTR_L(x) CONCAT(L, x)
#else
#include <getopt.h>
#define WSTR_L(x) x
#define option_w option
#define getopt_long_w getopt_long
#define optarg_w optarg
#define wchar_t char
#define wcstod strtod
#define wcstol strtol
#define wcstok(S, D, P) strtok(S, D)
#endif

using namespace msu;

AudioBase::AudioBase() :
	m_infile(WSTR_L("")), m_outfile(WSTR_L("")),
	m_trim_start(0), m_trim_end(0), m_start_offset(0), m_loop(0),
	m_fade_in(0), m_fade_out(0), m_cross_fade(0),
	m_pad_start(0), m_pad_end(0),
	m_tempo(-1.0), m_normalization(0.0), m_compression(false),
	m_dither_type('n')
{

}


AudioBase::AudioBase(std::fstring_t in): AudioBase()
{
	m_infile = in;
}


AudioBase::AudioBase(std::fstring_t in, std::fstring_t out) : AudioBase()
{
	m_infile = in;
	m_outfile = out;
}


AudioBase::AudioBase(int argc, char** argv): AudioBase()
{
    struct option_w longopts[] = {
        { WSTR_L("file"),          required_argument,  NULL,   WSTR_L('i') },
        { WSTR_L("output"),        required_argument,  NULL,   WSTR_L('o') },
        { WSTR_L("loop"),          required_argument,  NULL,   WSTR_L('l') },
        { WSTR_L("trim"),          required_argument,  NULL,   WSTR_L('t') },
        { WSTR_L("fade"),          required_argument,  NULL,   WSTR_L('f') },
        { WSTR_L("crossfade"),     required_argument,  NULL,   WSTR_L('x') },
        { WSTR_L("pad"),           required_argument,  NULL,   WSTR_L('p') },
        { WSTR_L("tempo"),         required_argument,  NULL,   WSTR_L('m') },
        { WSTR_L("normalization"), required_argument,  NULL,   WSTR_L('n') },
        { WSTR_L("compression"),   no_argument,        NULL,   WSTR_L('c') },
        { 0,                0,                  0,      0 }
    };

    wchar_t** _argv = new wchar_t*[argc + 1];
    _argv[0] = WSTR_L("AudioBase");
    int _argc = 1;

    for (auto i = 0; i < argc; ++i)
    {
        if (argv[i][0] == '[')
        {
            if (argv[i][strlen(argv[i]) - 1] != ']')
            {
                ++i;
                for (int depth = 1; i < argc; ++i)
                {
                    if (argv[i][strlen(argv[i]) - 1] == ']')
                        --depth;

                    if (depth == 0)
                        break;

                    if (argv[i][0] == '[')
                        ++depth;
                }
            }
        }

        else if (argv[i][0] == '(')
        {
            if (argv[i][strlen(argv[i]) - 1] != ')')
            {
                ++i;
                for (int depth = 1; i < argc; ++i)
                {
                    if (argv[i][strlen(argv[i]) - 1] == ')')
                        --depth;

                    if (depth == 0)
                        break;

                    if (argv[i][0] == '(')
                        ++depth;
                }
            }
        }

        else
        {
#ifdef WIN32
            std::fstring_t arg = std::fstring_t(utf8_to_wstring.from_bytes(argv[i]));
#else
            std::string arg = std::string(argv[i]);
#endif
            _argv[_argc] = new wchar_t[arg.length() + 1]{ 0 };
            arg.copy(_argv[_argc++], arg.length());
        }
    }

    wchar_t c = wchar_t(0);
    int _optind = optind;
    optind = 0;
    while ((c = getopt_long_w(_argc, _argv, WSTR_L("i:o:l:t:f:x:p:m:n:c"), longopts, NULL)) != wchar_t(-1))
    {
#ifdef WIN32
        wchar_t* tok;
        wchar_t* wbuf;
#else
        char* tok;
#endif
        switch (c) {
        case WSTR_L('i'):  // Input file name
            m_infile = std::fstring_t(optarg_w);
            break;

        case WSTR_L('o'):  // Output file name
            m_outfile = std::fstring_t(optarg_w);
            break;

        case WSTR_L('l'):  // Loop point
            m_loop = wcstol(optarg_w, nullptr, 10);
            break;

        case WSTR_L('t'):  // Trim
            tok = wcstok(optarg_w, WSTR_L(":"), &wbuf);
            if (tok != WSTR_L(""))
                m_trim_start = wcstol(tok, nullptr, 10);

            tok = wcstok(nullptr, WSTR_L(":"), &wbuf);
            if (tok != nullptr && tok != WSTR_L(""))
                m_trim_end = wcstol(tok, nullptr, 10);

            break;

        case WSTR_L('f'):  // Fade
            tok = wcstok(optarg_w, WSTR_L(":"), &wbuf);
            if (tok != WSTR_L(""))
                m_fade_in = wcstol(tok, nullptr, 10);

            tok = wcstok(nullptr, WSTR_L(":"), &wbuf);
            if (tok != nullptr && tok != WSTR_L(""))
                m_fade_out = wcstol(tok, nullptr, 10);

            break;

        case WSTR_L('x'):  // Cross-fade
            m_cross_fade = wcstol(optarg_w, nullptr, 10);
            break;

        case WSTR_L('p'):  // Pad
            tok = wcstok(optarg_w, WSTR_L(":"), &wbuf);
            if (tok != WSTR_L(""))
                m_pad_start = wcstol(tok, nullptr, 10);

            tok = wcstok(nullptr, WSTR_L(":"), &wbuf);
            if (tok != nullptr && tok != WSTR_L(""))
                m_pad_end = wcstol(tok, nullptr, 10);

            break;

        case WSTR_L('m'):  // Tempo
            m_tempo = wcstod(optarg_w, nullptr);
            break;

        case WSTR_L('n'):  // Normalization
            m_normalization = wcstod(optarg_w, nullptr);
            break;

        case WSTR_L('c'):  // Compression
            m_compression = true;
            break;

        case 0:
        default:
            break;
        }
    }

    optind = _optind;

    for (auto i = 1; i < _argc; ++i)
        delete _argv[i];
    delete[] _argv;
}


AudioBase::AudioBase(const AudioBase& a) : AudioBase()
{
	*this = a;
}


AudioBase::~AudioBase()
{
}


AudioBase& AudioBase::operator=(const AudioBase& a)
{
	m_infile = a.m_infile;
	m_outfile = a.m_outfile;
	m_trim_start = a.m_trim_start;
	m_trim_end = a.m_trim_end;
	m_start_offset = a.m_start_offset;
	m_loop = a.m_loop;
	m_fade_in = a.m_fade_in;
	m_fade_out = a.m_fade_out;
	m_cross_fade = a.m_cross_fade;
	m_pad_start = a.m_pad_start;
	m_pad_end = a.m_pad_end;
	m_tempo = a.m_tempo;
	m_normalization = a.m_normalization;
	m_compression = a.m_compression;
	m_dither_type = a.m_dither_type;
	return *this;
}


void AudioBase::clear()
{
	m_infile.clear();
	m_outfile.clear();
	m_trim_start = 0;
	m_trim_end = 0;
	m_start_offset = 0;
	m_loop = 0;
	m_fade_in = 0;
	m_fade_out = 0;
	m_cross_fade = 0;
	m_pad_start = 0;
	m_pad_end = 0;
	m_tempo = 1.0;
	m_normalization = 0.0;
	m_compression = false;
	m_dither_type = 'n';
}


void AudioBase::render()
{
	if (!m_infile.empty())
	{
		SoxWrapper* sox = SoxWrapperFactory::getInstance();

        // Read existing loop point from PCM inputs if one isn't explicitly specified
        if (m_infile.substr(m_infile.length() - 4).compare(WSTR_L(".pcm")) == 0 && m_loop == 0)
        {
#ifdef WIN32
            std::ifstream infile(utf8_to_wstring.to_bytes(m_infile).c_str(), std::ios::in | std::ios::binary);
#else
            std::ifstream infile(m_infile.c_str(), std::ios::in | std::ios::binary);
#endif
            if (infile.is_open())
            {
                char signature[4];
                infile.read(signature, 4);  // Verify file signature
                if (strncmp(signature, "MSU1", 4) == 0)
                {
                    infile.read((char*)&m_loop, sizeof(m_loop));
                }
            }
            infile.close();
        }

		if (m_loop && m_trim_start > m_loop)
		{
			m_start_offset = m_trim_start - m_loop;
			m_trim_start = m_loop;
		}

		if(sox->init(m_infile, m_outfile))
		{
			if (m_compression)
				sox->compress();
			if (sox->crossFade(m_loop, m_trim_end, m_cross_fade))
				m_trim_end = 0;
			sox->trim(m_trim_start, m_trim_end);
			sox->normalize(m_normalization);
			sox->fade(m_fade_in, m_fade_out);
			sox->pad(m_pad_start, m_pad_end);
			sox->tempo(m_tempo);
			sox->loop(m_trim_start + m_start_offset, m_loop + m_pad_start);
			sox->dither(m_dither_type);
			sox->finalize();
		}
		else if (config.verbosity() > 0)
		{
#ifdef WIN32
			std::wcout << WSTR_L("Error opening input file ") << m_infile << std::endl;
#else
			std::cout << WSTR_L("Error opening input file ") << m_infile << std::endl;
#endif
		}
	}
}


const std::fstring_t& AudioBase::inFile() const
{
	return m_infile;
}


std::fstring_t& AudioBase::inFile()
{
	return m_infile;
}


const std::fstring_t& AudioBase::outFile() const
{
	return m_outfile;
}


std::fstring_t& AudioBase::outFile()
{
	return m_outfile;
}


int AudioBase::loop() const
{
	return m_loop;
}


int& AudioBase::loop()
{
	return m_loop;
}


int AudioBase::trimStart() const
{
	return m_trim_start;
}


int& AudioBase::trimStart()
{
	return m_trim_start;
}


int AudioBase::trimEnd() const
{
	return m_trim_end;
}


int& AudioBase::trimEnd()
{
	return m_trim_end;
}


int AudioBase::fadeIn() const
{
	return m_fade_in;
}


int& AudioBase::fadeIn()
{
	return m_fade_in;
}


int AudioBase::fadeOut() const
{
	return m_fade_out;
}


int& AudioBase::fadeOut()
{
	return m_fade_out;
}


int AudioBase::crossFade() const
{
	return m_cross_fade;
}


int& AudioBase::crossFade()
{
	return m_cross_fade;
}

int AudioBase::padStart() const
{
	return m_pad_start;
}


int& AudioBase::padStart()
{
	return m_pad_start;
}


int AudioBase::padEnd() const
{
	return m_pad_end;
}


int& AudioBase::padEnd()
{
	return m_pad_end;
}


double AudioBase::tempo() const
{
	return m_tempo;
}


double& AudioBase::tempo()
{
	return m_tempo;
}


double AudioBase::normalization() const
{
	return m_normalization;
}


double& AudioBase::normalization()
{
	return m_normalization;
}


bool AudioBase::compression() const
{
	return m_compression;
}


bool& AudioBase::compression()
{
	return m_compression;
}
