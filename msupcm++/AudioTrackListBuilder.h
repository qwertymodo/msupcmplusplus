#pragma once
#include "AudioTrackList.h"
#include <string>

namespace msu
{
	class AudioTrackListBuilder
	{
	public:
		AudioTrackListBuilder(std::wstring config = L"tracks.json");

		const AudioTrackList& get() const;
		AudioTrackList& get();

	private:
		AudioTrackList m_list;
	};
}