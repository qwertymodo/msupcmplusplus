#include "SoxWrapper.h"
#include "GlobalConfig.h"
#include <assert.h>

using namespace msu;

SoxWrapper* SoxWrapperFactory::m_instance(0);

SoxWrapper::SoxWrapper():
	m_initialized(false), m_finalized(false),
	m_in(0), m_num_in(0), m_out(0), m_chain(0),
	m_out_signal({ 44100, 2, 16, 0, NULL }),
	m_out_encoding({ SOX_ENCODING_SIGN2, 16, NULL, sox_option_no, sox_option_no, sox_option_no, sox_false })
{
	sox_init();
	sox_get_globals()->verbosity = GlobalConfig::verbosity();
}


SoxWrapper::~SoxWrapper()
{
	clear();
	sox_quit();
}

bool SoxWrapper::init(std::string in, std::string out)
{
	if (!clear())
		return false;

	m_in = new sox_format_t*[1];
	
	m_in[0] = sox_open_read(in.c_str(), NULL, NULL, NULL);
	m_num_in = 1;

	m_in_signal = m_in[0]->signal;

	m_out = sox_open_write(out.c_str(), &m_out_signal, &m_out_encoding, "wav", NULL, NULL);

	m_chain = sox_create_effects_chain(&m_in[0]->encoding, &m_out_encoding);

	if (addEffect("input", 1, (char**)m_in))
	{
		m_in_signal.length = m_in[0]->signal.length;
		m_initialized = true;
	}

	return m_initialized;
}


bool SoxWrapper::trim(unsigned start, unsigned end)
{
	char* args[2];
	bool ret;

	if (!m_initialized || m_finalized)
		return false;

	if (start > 0 || end > 0)
	{
		args[0] = new char[32];
		args[1] = new char[32];

		if (start > 0)
		{
			strncpy(args[0], std::to_string(start).append("s").c_str(), 32);
		}
		else
		{
			strncpy(args[0], "0s", 32);
		}

		if (end > 0)
		{
			strncpy(args[1], std::string("=").append(std::to_string(end)).append("s").c_str(), 32);
			ret = addEffect("trim", 2, (char**)args);
		}
		else
		{
			ret = addEffect("trim", 1, (char**)args);
		}

		delete(args[0]);
		delete(args[1]);

		return ret;
	}

	return false;
}


bool SoxWrapper::fade(unsigned in, unsigned out, char type)
{
	char* args[4];
	bool ret;

	if (!m_initialized || m_finalized)
		return false;

	if (std::string("qhtlp").find(type) == std::string::npos)
		return false;

	if (in > 0 || out > 0)
	{
		args[0] = new char[2] { type, '\0' };
		args[1] = new char[32];
		args[2] = new char[3] { '-', '0', '\0' };
		args[3] = new char[32];

		if (in > 0)
		{
			strncpy(args[1], std::to_string(in).append("s").c_str(), 32);
		}
		else
		{
			strncpy(args[1], "0", 32);
		}

		if (out > 0)
		{

			strncpy(args[3], std::to_string(out).append("s").c_str(), 32);
			ret = addEffect("fade", 4, (char**)args);
		}
		else
		{
			ret = addEffect("fade", 2, (char**)args);
		}

		delete(args[0]);
		delete(args[1]);
		delete(args[2]);
		delete(args[3]);

		return ret;
	}

	return false;
}


bool SoxWrapper::finalize()
{
	char* args[1];
	args[0] = (char*)m_out;

	if (!addEffect("output", 1, args))
		return false;

	if (!sox_flow_effects(m_chain, NULL, NULL) == SOX_SUCCESS)
		return false;

	clear();
	return true;
}


bool SoxWrapper::clear()
{
	for (auto i = 0; i < m_num_in; ++i)
	{
		if (m_in[i])
		{
			sox_close(m_in[i]);
			m_in[i] = 0;
		}

		m_num_in = 0;
	}

	if (m_in)
	{
		delete(m_in);
		m_in = 0;
	}

	if (m_out)
	{
		sox_close(m_out);
		m_out = 0;
	}

	if (m_chain)
	{
		sox_delete_effects_chain(m_chain);
		m_chain = 0;
	}

	m_initialized = false;
	m_finalized = false;

	return true;
}


bool SoxWrapper::addEffect(std::string name, int argc, char** argv, int in_id)
{
	sox_effect_t* e = sox_create_effect(sox_find_effect(name.c_str()));

	if (!(sox_effect_options(e, argc, argv) == SOX_SUCCESS))
		return false;

	if (!(sox_add_effect(m_chain, e, &m_in_signal, &m_out_signal) == SOX_SUCCESS))
		return false;

	free(e);

	return true;
}