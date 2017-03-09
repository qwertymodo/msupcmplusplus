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
		static int& normalization();
		static unsigned int& verbosity();
		static bool& keep_temps();

	private:
		static std::string m_game;
		static std::string m_pack;
		static std::string m_artist;
		static std::string m_url;
		static std::string m_output_prefix;
		static int m_normalization;
		static unsigned int m_verbosity;
		static bool m_keep_temps;
	} config;
}
