#include "AudioBase.h"
#include "GlobalConfig.h"
#include "SoxWrapper.h"

using namespace msu;

AudioBase::AudioBase() :
	m_infile(""), m_outfile(""),
	m_trim_start(0), m_trim_end(0), m_loop(0),
	m_fade_in(0), m_fade_out(0), m_cross_fade(0),
	m_pad_start(0), m_pad_end(0),
	m_tempo(-1.0)
{

}


AudioBase::AudioBase(const char *in): AudioBase()
{
	m_infile = in;
}


AudioBase::AudioBase(const char *in, const char *out) : AudioBase()
{
	m_infile = in;
	m_outfile = out;
}


AudioBase::~AudioBase()
{
}


void AudioBase::render()
{
	SoxWrapper* sox = SoxWrapperFactory::getInstance();

	sox->init(m_infile, m_outfile);
	sox->trim(m_trim_start, m_trim_end);
	sox->fade(m_fade_in, m_fade_out);
	sox->pad(m_pad_start, m_pad_end);
	sox->tempo(m_tempo);
	sox->finalize();
}


const std::string& AudioBase::inFile() const
{
	return m_infile;
}


std::string& AudioBase::inFile()
{
	return m_infile;
}


const std::string& AudioBase::outFile() const
{
	return m_outfile;
}


std::string& AudioBase::outFile()
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
