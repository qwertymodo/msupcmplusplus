#include "AudioBase.h"
#include "GlobalConfig.h"
#include "SoxWrapper.h"
#include <iostream>

using namespace msu;

AudioBase::AudioBase() :
	m_infile(L""), m_outfile(L""),
	m_trim_start(0), m_trim_end(0), m_start_offset(0), m_loop(0),
	m_fade_in(0), m_fade_out(0), m_cross_fade(0),
	m_pad_start(0), m_pad_end(0),
	m_tempo(-1.0), m_normalization(0.0), m_compress(false),
	m_dither_type('n')
{

}


AudioBase::AudioBase(std::wstring in): AudioBase()
{
	m_infile = in;
}


AudioBase::AudioBase(std::wstring in, std::wstring out) : AudioBase()
{
	m_infile = in;
	m_outfile = out;
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
	m_compress = a.m_compress;
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
	m_compress = false;
	m_dither_type = 'n';
}


void AudioBase::render()
{
	if (!m_infile.empty())
	{
		SoxWrapper* sox = SoxWrapperFactory::getInstance();

		if (m_loop && m_trim_start > m_loop)
		{
			m_start_offset = m_trim_start - m_loop;
			m_trim_start = m_loop;
		}

		if(sox->init(m_infile, m_outfile))
		{
			if (m_compress)
				sox->compress();
			if (sox->crossFade(m_loop, m_trim_end, m_cross_fade))
				m_trim_end = 0;
			sox->trim(m_trim_start, m_trim_end);
			sox->normalize(m_normalization);
			sox->fade(m_fade_in, m_fade_out);
			sox->pad(m_pad_start, m_pad_end);
			sox->tempo(m_tempo);
			sox->setLoop(m_trim_start + m_start_offset, m_loop);
			sox->dither(m_dither_type);
			sox->finalize();
		}
		else if (config.verbosity() > 0)
		{
			std::wcout << L"Error opening input file " << m_infile << std::endl;
		}
	}
}


const std::wstring& AudioBase::inFile() const
{
	return m_infile;
}


std::wstring& AudioBase::inFile()
{
	return m_infile;
}


const std::wstring& AudioBase::outFile() const
{
	return m_outfile;
}


std::wstring& AudioBase::outFile()
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


bool AudioBase::compress() const
{
	return m_compress;
}


bool& AudioBase::compress()
{
	return m_compress;
}
