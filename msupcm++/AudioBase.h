#pragma once
#include <string>

namespace msu
{
	class AudioBase
	{
	public:
		AudioBase();
		AudioBase(const char *in);
		AudioBase(const char *in, const char *out);
		~AudioBase();

		virtual void render();

		const std::string& id() const;
		std::string& id();

		const std::string& inFile() const;
		std::string& inFile();

		const std::string& outFile() const;
		std::string& outFile();

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

	private:
		std::string m_id;
		std::string m_infile;
		std::string m_outfile;
		int m_trim_start;
		int m_trim_end;
		int m_fade_in;
		int m_fade_out;
		int m_cross_fade;
		int m_pad_start;
		int m_pad_end;
		double m_tempo;
	};
}
