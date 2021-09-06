#pragma once
#include "AudioBase.h"
#include "AudioSubChannel.h"

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
		AudioSubTrack(std::fstring_t in);
		AudioSubTrack(std::fstring_t in, std::fstring_t out);
		AudioSubTrack(int argc, char** argv);
		AudioSubTrack(const AudioSubTrack& a);
		~AudioSubTrack();

		AudioSubTrack& operator=(const AudioSubTrack& a);
		AudioSubTrack& operator=(const AudioBase& a);

		virtual void clear() override;

		virtual void render() override;

		AudioBase* subChannels() const;
		int numSubChannels() const;

		void addSubChannel(AudioBase* a);

	protected:
		AudioBase* m_sub_channels;
		int m_num_sub_channels;
	};
}
