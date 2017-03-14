#pragma once
#include "AudioBase.h"
#include "AudioSubChannel.h"
#include "AudioSubTrack.h"

/**
* An AudioTrack is the main AudioBase type.  Each output track corresponds
* to a single AudioTrack obsject.  An AudioTrack may contain one or more
* AudioChannels or AudioSubTracks, but it should not directly contain both,
* however either one is fully nestable.
*/

namespace msu
{
	class AudioTrack : public AudioSubChannel, public AudioSubTrack
	{
	public:
		AudioTrack();
		AudioTrack(const char *in);
		AudioTrack(const char *in, const char *out);
		AudioTrack(const AudioTrack& a);
		~AudioTrack();

		AudioTrack& operator=(const AudioTrack& a);
		AudioTrack& operator=(const AudioSubChannel& a);
		AudioTrack& operator=(const AudioSubTrack& a);
		AudioTrack& operator=(const AudioBase& a);

		virtual void clear();

		virtual void render() final;

		int trackNumber() const;
		int& trackNumber();

		const std::string& title() const;
		std::string& title();

	protected:
		int m_track_number;
		std::string m_title;
	};
}
