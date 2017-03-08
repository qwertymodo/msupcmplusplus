#pragma once
#include "AudioBase.h"

/**
* An AudioChannel is a type of AudioBase which will be mixed with other
* AudioChannels to form the final track. AudioChannels may contain 0 or
* more AudioSubTracks.
*/

namespace msu
{
	class AudioChannel : public virtual AudioBase
	{
	public:
		AudioChannel();
		AudioChannel(const char *in);
		AudioChannel(const char *in, const char *out);
		~AudioChannel();

		virtual void render() override;

		AudioBase* subtracks() const;
		int numSubtracks() const;

		void addSubtrack(const AudioBase& a);
		void removeSubtrack(int idx);

	protected:
		AudioBase* m_subtracks;
		int m_num_subtracks;
	};
}
