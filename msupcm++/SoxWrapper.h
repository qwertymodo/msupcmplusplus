#pragma once
#include "sox_main.h"
#include <codecvt>
#include <string>

namespace msu
{
	class SoxWrapper
	{
	public:
		SoxWrapper();
		~SoxWrapper();

		bool init(std::wstring in, std::wstring out);
		bool addInput(std::wstring name);
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
		bool addOutput(std::wstring name);
		bool addEffect(std::string name, int argc, char** argv);
		std::wstring getTempFile(std::wstring ext);

		bool m_initialized;
		bool m_finalized;
		int m_temp_counter;
		double m_tempo;
		size_t m_loop;
		size_t m_length;
		sox_rate_t m_input_rate;
		char m_dither_type;

		std::wstring m_output;
		std::wstring_convert<std::codecvt_utf8<wchar_t>> wchar_to_utf8;
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