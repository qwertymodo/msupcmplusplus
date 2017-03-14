#pragma once
#include "AudioBase.h"
#include "AudioSubTrack.h"

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
		AudioSubChannel(const AudioSubChannel& a);
		~AudioSubChannel();

		AudioSubChannel& operator=(const AudioSubChannel& a);
		AudioSubChannel& operator=(const AudioBase& a);
		
		virtual void clear() override;

		virtual void render() override;

		AudioBase* subTracks() const;
		int numSubTracks() const;

		void addSubTrack(AudioBase* a);

	protected:
		AudioBase* m_sub_tracks;
		int m_num_sub_tracks;
	};
}
