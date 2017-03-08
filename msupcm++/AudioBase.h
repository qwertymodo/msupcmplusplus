#pragma once
#include "sox.h"
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

	private:
		std::string m_id;
		std::string m_infile;
		std::string m_outfile;
		int m_trim_start;
		int m_trim_end;
	};
}