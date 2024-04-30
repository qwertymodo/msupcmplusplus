#pragma once
#include <string>

#ifdef WIN32
#define fstring_t wstring
#else
#define fstring_t string
#endif

namespace msu
{
	static class GlobalConfig
	{
	public:
		static std::wstring& game();
		static std::wstring& pack();
		static std::wstring& artist();
		static std::wstring& url();
		static std::fstring_t& output_prefix();
		static double& normalization();
		static bool& dither();
		static unsigned int& verbosity();
		static bool& keep_temps();
		static int& first_track();
		static int& last_track();
		static bool& use_render_track_field();

	private:
		static std::wstring m_game;
		static std::wstring m_pack;
		static std::wstring m_artist;
		static std::wstring m_url;
		static std::fstring_t m_output_prefix;
		static double m_normalization;
		static bool m_dither;
		static unsigned int m_verbosity;
		static bool m_keep_temps;
		static int m_first_track;
		static int m_last_track;
		static bool m_use_render_track_field;
	} config;
}
