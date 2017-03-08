#pragma once
#include "AudioTrack.h"
#include "GlobalConfig.h"
#include <string>
#include <vector>

namespace msu
{
	class AudioTrackList
	{
	public:
		AudioTrackList();
		~AudioTrackList();

		void render();

		const std::vector<AudioTrack>& tracks() const;
		std::vector<AudioTrack>& tracks();

	private:
		std::vector<AudioTrack> m_tracks;
	};
}
