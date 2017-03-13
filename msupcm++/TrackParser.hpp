#pragma once
#include "json.hpp"
#include "AudioTrackList.h"
#include "GlobalConfig.h"
#include <vector>

using nlohmann::json;

namespace msu {
	void to_json(json& j, const AudioBase& a)
	{
		j = json{
			{ "file", a.inFile() },
			{ "output", a.outFile() },
			{ "trim_start", a.trimStart() },
			{ "trim_end", a.trimEnd() },
			{ "loop", a.loop() },
			{ "fade_in", a.fadeIn() },
			{ "fade_out", a.fadeIn() },
			{ "cross_fade", a.fadeIn() },
			{ "pad_start", a.padStart() },
			{ "pad_end", a.padEnd() },
			{ "tempo", a.tempo() },
			{ "normalization", a.normalization() }
		};
	}


	void to_json(json& j, const AudioChannel& a)
	{
		to_json(j, dynamic_cast<const AudioBase&>(a));
		
		for (auto i = 0; i < a.numSubtracks(); ++i)
		{
			json subtrack;
			to_json(subtrack, a.subtracks()[i]);

			j["sub_tracks"].push_back(subtrack);
		}
	}


	void to_json(json &j, const AudioSubTrack& a)
	{
		to_json(j, dynamic_cast<const AudioBase&>(a));

		for (auto i = 0; i < a.numChannels(); ++i)
		{
			json channel;
			to_json(channel, a.channels()[i]);

			j["sub_channels"].push_back(channel);
		}
	}


	void to_json(json &j, const AudioTrack& a)
	{
		if (a.numChannels() > 0)
		{
			to_json(j, dynamic_cast<const AudioSubTrack&>(a));
		}
		else if (a.numSubtracks() > 0)
		{
			to_json(j, dynamic_cast<const AudioChannel&>(a));
		}
		else
		{
			to_json(j, dynamic_cast<const AudioBase&>(a));
		}

		j["track_number"] = a.trackNumber();
		j["title"] = a.title();
	}


	void to_json(json &j, const AudioTrackList& a)
	{
		j = json{
			{ "game", config.game() },
			{ "pack", config.pack() },
			{ "artist", config.artist() },
			{ "url", config.url() },
			{ "output_prefix", config.output_prefix() },
			{ "normalization", config.normalization() },
			{ "verbosity", config.verbosity() },
			{ "keep_temps", config.keep_temps() }
		};

		for (auto i = a.tracks().begin(); i != a.tracks().end(); ++i)
		{
			json track;
			to_json(track, *i);
			j["tracks"].push_back(track);
		}
	}


	void from_json(const json& j, AudioBase& a)
	{
		if (j.find("file") != j.end())
			a.inFile() = j["file"].get<std::string>();

		if (j.find("output") != j.end())
			a.outFile() = j["output"].get<std::string>();

		if (j.find("trim_start") != j.end())
			a.trimStart() = j["trim_start"].get<int>();

		if (j.find("trim_end") != j.end())
			a.trimEnd() = j["trim_end"].get<int>();

		if (j.find("loop") != j.end())
			a.loop() = j["loop"].get<int>();

		if (j.find("fade_in") != j.end())
			a.fadeIn() = j["fade_in"].get<int>();

		if (j.find("fade_out") != j.end())
			a.fadeOut() = j["fade_out"].get<int>();

		if (j.find("cross_fade") != j.end())
			a.crossFade() = j["cross_fade"].get<int>();

		if (j.find("pad_start") != j.end())
			a.padStart() = j["pad_start"].get<int>();

		if (j.find("pad_end") != j.end())
			a.padEnd() = j["pad_end"].get<int>();

		if (j.find("tempo") != j.end())
			a.tempo() = j["tempo"].get<double>();

		if (j.find("normalization") != j.end())
		{
			a.normalization() = j["normalization"].get<double>();
		}
		else
		{
			a.normalization() = config.normalization();
		}
	}


	void from_json(const json& j, AudioChannel& a)
	{
		from_json(j, dynamic_cast<AudioBase&>(a));

		if (j.find("sub_tracks") != j.end())
		{
			for (auto i = j["sub_tracks"].begin(); i != j["sub_tracks"].end(); ++i)
			{
				AudioSubTrack s = *i;
				a.addSubtrack(dynamic_cast<const AudioBase &>(s));
			}
		}
	}


	void from_json(const json& j, AudioSubTrack& a)
	{
		from_json(j, dynamic_cast<AudioBase&>(a));

		if (j.find("sub_channels") != j.end())
		{
			for (auto i = j["sub_channels"].begin(); i != j["sub_channels"].end(); ++i)
			{
				AudioChannel c = *i;
				a.addChannel(dynamic_cast<const AudioBase &>(c));
			}
		}
	}


	void from_json(const json& j, AudioTrack& a)
	{
		if (j.find("sub_channels") != j.end())
		{
			from_json(j, dynamic_cast<AudioSubTrack&>(a));
		}
		else if (j.find("sub_tracks") != j.end())
		{
			from_json(j, dynamic_cast<AudioChannel&>(a));
		}
		else
		{
			from_json(j, dynamic_cast<AudioBase&>(a));
		}

		if (j.find("track_number") != j.end())
			a.trackNumber() = j["track_number"].get<int>();

		if (j.find("title") != j.end())
			a.title() = j["title"].get<std::string>();

		if (a.outFile().empty())
			a.outFile() = GlobalConfig::output_prefix() + "-" + std::to_string(a.trackNumber()) + ".pcm";
	}


	void from_json(const json& j, AudioTrackList& a)
	{
		if (j.find("game") != j.end())
			config.game() = j["game"].get<std::string>();

		if (j.find("pack") != j.end())
			config.pack() = j["pack"].get<std::string>();

		if (j.find("artist") != j.end())
			config.artist() = j["artist"].get<std::string>();

		if (j.find("url") != j.end())
			config.url() = j["url"].get<std::string>();

		if (j.find("output_prefix") != j.end())
		{
			config.output_prefix() = j["output_prefix"].get<std::string>();
		}
		else
		{
			config.output_prefix() = "track";
		}

		if (j.find("normalization") != j.end())
			config.normalization() = j["normalization"].get<int>();

		if (j.find("verbosity") != j.end())
			config.verbosity() = j["verbosity"].get<unsigned int>();

		if (j.find("keep_temps") != j.end())
			config.keep_temps() = j["keep_temps"].get<bool>();

		if (j.find("tracks") != j.end())
		{
			for (auto i = j["tracks"].begin(); i != j["tracks"].end(); ++i)
			{
				a.tracks().push_back(*i);
			}
		}
	}
}
