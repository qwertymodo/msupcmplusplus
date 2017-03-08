#include "AudioBase.h"
#include "GlobalConfig.h"
#include <assert.h>
#include <stdio.h>

using namespace msu;

AudioBase::AudioBase():
	m_id(""), m_infile(""), m_outfile(""), m_trim_start(0), m_trim_end(0)
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


void addEffect(sox_effects_chain_t* chain, sox_format_t* in, sox_format_t* out, std::string name, int argc, char** argv)
{
	sox_effect_t* e = sox_create_effect(sox_find_effect(name.c_str()));
	assert(sox_effect_options(e, argc, argv) == SOX_SUCCESS);
	assert(sox_add_effect(chain, e, &in->signal, &out->signal) == SOX_SUCCESS);
	free(e);
}


void AudioBase::render()
{
	sox_format_t* infile;
	sox_format_t* outfile;
	sox_effects_chain_t* fxchain;
	sox_signalinfo_t outsignal{ 44100, 2, 16, 0, NULL };
	sox_encodinginfo_t outencoding{ SOX_ENCODING_SIGN2, 16, NULL, sox_option_no, sox_option_no, sox_option_no, sox_false};

	sox_get_globals()->verbosity = config.verbosity();

	unsigned bufsize = 255;
	char* args[10];
	char* argbuf1, * argbuf2;
	argbuf1 = new char[bufsize];
	argbuf2 = new char[bufsize];

	assert(infile = sox_open_read(m_infile.c_str(), NULL, NULL, NULL));
	assert(outfile = sox_open_write(m_outfile.c_str(), &outsignal, &outencoding, NULL, NULL, NULL));

	fxchain = sox_create_effects_chain(&infile->encoding, &outfile->encoding);

	args[0] = (char*)infile;
	addEffect(fxchain, infile, outfile, "input", 1, args);

	if (m_trim_start > 0 || m_trim_end > 0)
	{
		if (m_trim_start > 0)
		{
			strncpy(argbuf1, std::to_string(m_trim_start).append("s").c_str(), bufsize);
		}
		else
		{
			strncpy(argbuf1, "0s", bufsize);
		}

		args[0] = argbuf1;

		if (m_trim_end > 0)
		{
			strncpy(argbuf2, std::string("=").append(std::to_string(m_trim_end)).append("s").c_str(), bufsize);
			args[1] = argbuf2;

			addEffect(fxchain, infile, outfile, "trim", 2, args);
		}
		else
		{
			addEffect(fxchain, infile, outfile, "trim", 1, args);
		}
	}

	args[0] = (char*)outfile;
	addEffect(fxchain, infile, outfile, "output", 1, args);

	sox_flow_effects(fxchain, NULL, NULL);
	sox_delete_effects_chain(fxchain);

	sox_close(infile);
	sox_close(outfile);
}
