#pragma once
#include <string>

namespace msu
{
	class AudioBase
	{
	public:
		AudioBase();
		AudioBase(std::wstring in);
		AudioBase(std::wstring in, std::wstring out);
		AudioBase(int argc, char** argv);
		AudioBase(const AudioBase& a);
		~AudioBase();

		AudioBase& operator=(const AudioBase& a);

		virtual void clear();

		virtual void render();

		const std::wstring& inFile() const;
		std::wstring& inFile();

		const std::wstring& outFile() const;
		std::wstring& outFile();

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
		std::wstring m_infile;
		std::wstring m_outfile;
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
