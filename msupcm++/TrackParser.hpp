#pragma once
#include "json.hpp"
#include "utf8.h"
#include "AudioTrackList.h"
#include "GlobalConfig.h"
#include <codecvt>
#include <string>
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
			{ "normalization", a.normalization() },
		};
	}


	void to_json(json& j, const AudioSubChannel& a)
	{
		to_json(j, (const AudioBase&)(a));
		
		for (auto i = 0; i < a.numSubTracks(); ++i)
		{
			json subtrack;
			to_json(subtrack, a.subTracks()[i]);

			j["sub_tracks"].push_back(subtrack);
		}
	}


	void to_json(json &j, const AudioSubTrack& a)
	{
		to_json(j, (const AudioBase&)(a));

		for (auto i = 0; i < a.numSubChannels(); ++i)
		{
			json channel;
			to_json(channel, a.subChannels()[i]);

			j["sub_channels"].push_back(channel);
		}
	}


	void to_json(json &j, const AudioTrack& a)
	{
		if (a.numSubChannels() > 0)
		{
			to_json(j, (const AudioSubTrack&)(a));
		}
		else if (a.numSubTracks() > 0)
		{
			to_json(j, (const AudioSubChannel&)(a));
		}
		else
		{
			to_json(j, (const AudioBase&)(a));
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
			{ "dither", config.dither() },
			{ "verbosity", config.verbosity() },
			{ "keep_temps", config.keep_temps() },
			{ "first_track", config.first_track() },
			{ "last_track", config.last_track() }
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
			a.inFile() = utf8_to_wstring.from_bytes(j["file"].get<std::string>());

		if (j.find("output") != j.end())
			a.outFile() = utf8_to_wstring.from_bytes(j["output"].get<std::string>());

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
			a.normalization() = j["normalization"].get<double>();

		if (j.find("compression") != j.end())
			a.compression() = j["compression"].get<bool>();
	}


	class Option {
	public:
		Option() {}
		const int& option() const { return m_option; }
		int& option() { return m_option; }

	private:
		int m_option;
	};


	void from_json(const json& j, Option& o)
	{
		if (j.find("option") != j.end())
			o.option() = j["option"].get<int>();
	}


	void from_json(const json& j, AudioSubChannel& a)
	{
		from_json(j, (AudioBase&)(a));

		if (j.find("sub_tracks") != j.end())
		{
			for (auto i = j["sub_tracks"].begin(); i != j["sub_tracks"].end(); ++i)
			{
				AudioSubTrack s = *i;

				if (s.inFile().empty())
					s.inFile() = a.inFile();

				a.addSubTrack(&s);
			}
		}

		if (j.find("use_option") != j.end() && j.find("options") != j.end())
		{
			int opt = j["use_option"].get<int>();

			for (auto i = j["options"].begin(); i != j["options"].end(); ++i)
			{
				if (Option(*i).option() == opt)
				{
					from_json(*i, a);
					break;
				}
			}
		}
	}


	void from_json(const json& j, AudioSubTrack& a)
	{
		from_json(j, (AudioBase&)(a));

		if (j.find("sub_channels") != j.end())
		{
			for (auto i = j["sub_channels"].begin(); i != j["sub_channels"].end(); ++i)
			{
				AudioSubChannel s = *i;

				if (s.inFile().empty())
					s.inFile() = a.inFile();

				a.addSubChannel(&s);
			}
		}

		if (j.find("use_option") != j.end() && j.find("options") != j.end())
		{
			int opt = j["use_option"].get<int>();

			for (auto i = j["options"].begin(); i != j["options"].end(); ++i)
			{
				if (Option(*i).option() == opt)
				{
					from_json(*i, a);
					break;
				}
			}
		}
	}


	void from_json(const json& j, AudioTrack& a)
	{
		if (j.find("sub_channels") != j.end())
		{
			from_json(j, (AudioSubTrack&)(a));
		}
		else if (j.find("sub_tracks") != j.end())
		{
			from_json(j, (AudioSubChannel&)(a));
		}
		else
		{
			from_json(j, (AudioBase&)(a));
		}

		if (j.find("track_number") != j.end())
			a.trackNumber() = j["track_number"].get<int>();

		if (j.find("title") != j.end())
			a.title() = utf8_to_wstring.from_bytes(j["title"].get<std::string>());

		if (a.outFile().empty())
			a.outFile() = config.output_prefix() + L"-" + std::to_wstring(a.trackNumber()) + L".pcm";

		if (a.normalization() == 0.0)
			a.normalization() = config.normalization();

		if (j.find("use_option") != j.end() && j.find("options") != j.end())
		{
			int opt = j["use_option"].get<int>();

			for (auto i = j["options"].begin(); i != j["options"].end(); ++i)
			{
				if (Option(*i).option() == opt)
				{
					from_json(*i, a);
					break;
				}
			}
		}
	}


	void from_json(const json& j, AudioTrackList& a)
	{
		if (j.find("game") != j.end())
			config.game() = utf8_to_wstring.from_bytes(j["game"].get<std::string>());

		if (j.find("pack") != j.end())
			config.pack() = utf8_to_wstring.from_bytes(j["pack"].get<std::string>());

		if (j.find("artist") != j.end())
			config.artist() = utf8_to_wstring.from_bytes(j["artist"].get<std::string>());

		if (j.find("url") != j.end())
			config.url() = utf8_to_wstring.from_bytes(j["url"].get<std::string>());

		if (j.find("output_prefix") != j.end())
		{
			config.output_prefix() = utf8_to_wstring.from_bytes(j["output_prefix"].get<std::string>());
		}
		else
		{
			config.output_prefix() = L"track";
		}

		if (j.find("normalization") != j.end())
			config.normalization() = j["normalization"].get<int>();

		if (j.find("dither") != j.end())
			config.dither() = j["dither"].get<bool>();

		if (j.find("verbosity") != j.end())
			config.verbosity() = j["verbosity"].get<unsigned int>();

		if (j.find("keep_temps") != j.end())
			config.keep_temps() = j["keep_temps"].get<bool>();

		if (j.find("first_track") != j.end())
			config.first_track() = j["first_track"].get<int>();

		if (j.find("last_track") != j.end())
			config.last_track() = j["last_track"].get<int>();

		if (j.find("tracks") != j.end())
		{
			for (auto i = j["tracks"].begin(); i != j["tracks"].end(); ++i)
			{
				a.tracks().push_back(*i);
			}
		}
	}
}
