#pragma once
#include "AudioBase.h"

/**
* An AudioSubTrack is a type of AudioBase which will be concatenated with
* other AudioSubTracks to form the final track. AudioSubTracks may contain
* 0 or more AudioChannels.
*/

namespace msu
{
	class AudioSubTrack : public virtual AudioBase
	{
	public:
		AudioSubTrack();
		AudioSubTrack(const char *in);
		AudioSubTrack(const char *in, const char *out);
		~AudioSubTrack();

		virtual void render() override;

		AudioBase* subChannels() const;
		int numSubChannels() const;

		void addSubChannel(const AudioBase& a);
		void removeSubChannel(int idx);

	protected:
		AudioBase* m_sub_channels;
		int m_num_sub_channels;
	};
}
