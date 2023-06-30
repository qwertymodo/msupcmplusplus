#pragma once
#include <string>

#ifdef WIN32
#define fstring_t wstring
#else
#define fstring_t string
#endif

namespace msu
{
	class AudioBase
	{
	public:
		AudioBase();
		AudioBase(std::fstring_t in);
		AudioBase(std::fstring_t in, std::fstring_t out);
		AudioBase(int argc, char** argv);
		AudioBase(const AudioBase& a);
		~AudioBase();

		AudioBase& operator=(const AudioBase& a);

		virtual void clear();

		virtual void render();

		const std::fstring_t& inFile() const;
		std::fstring_t& inFile();

		const std::fstring_t& outFile() const;
		std::fstring_t& outFile();

		int loop() const;
		int& loop();

		int trimStart() const;
		int& trimStart();

		int trimEnd() const;
		int& trimEnd();

		int fadeIn() const;
		int& fadeIn();

		int fadeOut() const;
		int& fadeOut();

		int crossFade() const;
		int& crossFade();

		int padStart() const;
		int& padStart();

		int padEnd() const;
		int& padEnd();

		double tempo() const;
		double& tempo();

		double normalization() const;
		double& normalization();

		bool compression() const;
		bool& compression();

	protected:
		std::fstring_t m_infile;
		std::fstring_t m_outfile;
		int m_loop;
		int m_trim_start;
		int m_trim_end;
		int m_start_offset;
		int m_fade_in;
		int m_fade_out;
		int m_cross_fade;
		int m_pad_start;
		int m_pad_end;
		double m_tempo;
		double m_normalization;
		bool m_compression;
		char m_dither_type;
	};
}
