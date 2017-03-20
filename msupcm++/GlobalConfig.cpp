#include "GlobalConfig.h"

using namespace msu;

std::wstring GlobalConfig::m_game = L"";
std::wstring GlobalConfig::m_pack = L"";
std::wstring GlobalConfig::m_artist = L"";
std::wstring GlobalConfig::m_url = L"";
std::wstring GlobalConfig::m_output_prefix = L"";
double GlobalConfig::m_normalization = 0.0;
bool GlobalConfig::m_dither = true;
unsigned int GlobalConfig::m_verbosity = 1;
bool GlobalConfig::m_keep_temps = false;
int GlobalConfig::m_first_track = -1;
int GlobalConfig::m_last_track = -1;

std::wstring& GlobalConfig::game()
{
	return m_game;
}


std::wstring& GlobalConfig::pack()
{
	return m_pack;
}


std::wstring& GlobalConfig::artist()
{
	return m_artist;
}


std::wstring& GlobalConfig::url()
{
	return m_url;
}


std::wstring& GlobalConfig::output_prefix()
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


int& GlobalConfig::first_track()
{
	return m_first_track;
}



int& GlobalConfig::last_track()
{
	return m_last_track;
}
