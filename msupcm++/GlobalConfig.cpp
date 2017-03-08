#include "GlobalConfig.h"

using namespace msu;

std::string GlobalConfig::m_game = "";
std::string GlobalConfig::m_pack = "";
std::string GlobalConfig::m_artist = "";
std::string GlobalConfig::m_url = "";
std::string GlobalConfig::m_output_prefix = "";
int GlobalConfig::m_normalization = 0;
unsigned int GlobalConfig::m_verbosity = 1;

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


int& GlobalConfig::normalization()
{
	return m_normalization;
}


unsigned int& GlobalConfig::verbosity()
{
	return m_verbosity;
}
