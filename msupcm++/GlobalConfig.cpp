#include "GlobalConfig.h"

using namespace msu;

#ifdef WIN32
#define CONCAT(x, y) x##y
#define WSTR_L(x) CONCAT(L, x)
#else
#define WSTR_L(x) x
#endif

std::wstring GlobalConfig::m_game = L"";
std::wstring GlobalConfig::m_pack = L"";
std::wstring GlobalConfig::m_artist = L"";
std::wstring GlobalConfig::m_url = L"";
std::fstring_t GlobalConfig::m_output_prefix = WSTR_L("");
double GlobalConfig::m_normalization = 0.0;
bool GlobalConfig::m_dither = true;
unsigned int GlobalConfig::m_verbosity = 2;
bool GlobalConfig::m_keep_temps = false;
int GlobalConfig::m_first_track = -1;
int GlobalConfig::m_last_track = -1;
bool GlobalConfig::m_use_render_track_field = false;

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


std::fstring_t& GlobalConfig::output_prefix()
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

bool& GlobalConfig::use_render_track_field()
{
	return m_use_render_track_field;
}
