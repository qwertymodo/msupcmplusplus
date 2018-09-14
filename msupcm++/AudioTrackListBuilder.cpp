#include "AudioTrackListBuilder.h"
#include "GlobalConfig.h"
#include "TrackParser.hpp"
#include "utf8.h"
#include <exception>
#include <fstream>
#include <iostream>

using namespace msu;

AudioTrackListBuilder::AudioTrackListBuilder(std::wstring config)
{
	std::ifstream ifs(wchar_to_utf8(config.c_str()).c_str());
	if (ifs.is_open())
	{
		json j;
		try
		{
			ifs >> j;
		}
		catch (const std::exception& e)
		{
			std::wcout << e.what() << std::endl;
			exit(1);
		}

		m_list = j;
	}
}


const AudioTrackList& AudioTrackListBuilder::get() const
{
	return m_list;
}


AudioTrackList& AudioTrackListBuilder::get()
{
	return m_list;
}
