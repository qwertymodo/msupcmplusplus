#include "AudioBase.h"
#include "GlobalConfig.h"
#include "SoxWrapper.h"

using namespace msu;

AudioBase::AudioBase():
	m_id(""), m_infile(""), m_outfile(""),
	m_trim_start(0), m_trim_end(0)
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


const std::string& AudioBase::id() const
{
	return m_id;
}


std::string& AudioBase::id()
{
	return m_id;
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


void AudioBase::render()
{
	SoxWrapper* sox = SoxWrapperFactory::getInstance();

	sox->init(m_infile, m_outfile);
	sox->trim(m_trim_start, m_trim_end);
	sox->fade(m_fade_in, m_fade_out);
	sox->finalize();
}
