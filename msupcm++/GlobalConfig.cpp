#include "GlobalConfig.h"

using namespace msu;

std::string GlobalConfig::m_game = "";
std::string GlobalConfig::m_pack = "";
std::string GlobalConfig::m_artist = "";
std::string GlobalConfig::m_url = "";
std::string GlobalConfig::m_output_prefix = "";
double GlobalConfig::m_normalization = 0.0;
bool GlobalConfig::m_dither = true;
unsigned int GlobalConfig::m_verbosity = 1;
bool GlobalConfig::m_keep_temps = false;

std::string& GlobalConfig::game()
{
	return m_game;
}


std::string& GlobalConfig::pack()
{
	return m_pack;
}


std::string& GlobalConfig::artist()
{
	return m_artist;
}


std::string& GlobalConfig::url()
{
	return m_url;
}


std::string& GlobalConfig::output_prefix()
{
	return m_output_prefix;
}


double& GlobalConfig::normalization()
{
	return m_normalization;
}


bool& GlobalConfig::dither()
{
	return m_dither;
}


unsigned int& GlobalConfig::verbosity()
{
	return m_verbosity;
}


bool& GlobalConfig::keep_temps()
{
	return m_keep_temps;
}
