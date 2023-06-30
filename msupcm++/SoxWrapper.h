#pragma once
#include "sox_main.h"
#include <codecvt>
#include <string>

#ifdef WIN32
#define fstring_t wstring
#else
#define fstring_t string
#endif

namespace msu
{
	class SoxWrapper
	{
	public:
		SoxWrapper();
		~SoxWrapper();

		bool init(std::fstring_t in, std::fstring_t out);
		bool addInput(std::fstring_t name);
		bool combine(sox_combine_method method);
		bool trim(size_t start, size_t end = 0);
		bool fade(size_t in, size_t out = 0, char type = 't');
		bool pad(size_t start, size_t end = 0);
		bool tempo(double tempo);
		bool loop(size_t start, size_t loop);
		bool crossFade(size_t loop, size_t end, size_t length, double ratio = 0.5);
		bool normalize(double level);
		bool compress();
		bool dither(char type = 's');
		bool finalize();
		bool clear();

		double tempo();
		size_t length();
		sox_rate_t inputRate();

	private:
		bool addOutput(std::fstring_t name);
		bool addEffect(std::string name, int argc, char** argv);
		std::fstring_t getTempFile(std::fstring_t ext);

		bool m_initialized;
		bool m_finalized;
		int m_temp_counter;
		double m_tempo;
		size_t m_loop;
		size_t m_length;
		sox_rate_t m_input_rate;

		std::fstring_t m_output;
	};

	class SoxWrapperFactory
	{
	public:
		~SoxWrapperFactory() { delete m_instance; }
		static SoxWrapper* getInstance()
		{
			if (!m_instance)
				m_instance = new SoxWrapper();

			return m_instance;
		}

	private:
		SoxWrapperFactory();
		static SoxWrapper *m_instance;
	};
}