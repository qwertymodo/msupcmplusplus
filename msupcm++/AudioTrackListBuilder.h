#pragma once
#include "AudioTrackList.h"
#include <string>

namespace msu
{
	class AudioTrackListBuilder
	{
	public:
		AudioTrackListBuilder(std::string config = "tracks.json");

		const AudioTrackList& get() const;
		AudioTrackList& get();

	private:
		AudioTrackList m_list;
	};
}