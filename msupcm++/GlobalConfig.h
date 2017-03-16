#pragma once
#include <string>

namespace msu
{
	static class GlobalConfig
	{
	public:
		static std::string& game();
		static std::string& pack();
		static std::string& artist();
		static std::string& url();
		static std::string& output_prefix();
		static double& normalization();
		static bool& dither();
		static unsigned int& verbosity();
		static bool& keep_temps();
		static int& first_track();
		static int& last_track();

	private:
		static std::string m_game;
		static std::string m_pack;
		static std::string m_artist;
		static std::string m_url;
		static std::string m_output_prefix;
		static double m_normalization;
		static bool m_dither;
		static unsigned int m_verbosity;
		static bool m_keep_temps;
		static int m_first_track;
		static int m_last_track;
	} config;
}
