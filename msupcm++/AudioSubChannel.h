#pragma once
#include "AudioBase.h"

/**
* An AudioSubChannel is a type of AudioBase which will be mixed with other
* AudioSubChannels to form the final track. AudioSubChannels may contain 0 or
* more AudioSubTracks.
*/

namespace msu
{
	class AudioSubChannel : public virtual AudioBase
	{
	public:
		AudioSubChannel();
		AudioSubChannel(const char *in);
		AudioSubChannel(const char *in, const char *out);
		~AudioSubChannel();

		virtual void render() override;

		AudioBase* subtracks() const;
		int numSubtracks() const;

		void addSubtrack(const AudioBase& a);
		void removeSubtrack(int idx);

	protected:
		AudioBase* m_sub_tracks;
		int m_num_sub_tracks;
	};
}
