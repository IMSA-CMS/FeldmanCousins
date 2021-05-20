#ifndef FREQUENTIST_ANALYTIC_ANALYSIS_DEBUGCHANNELREADER_H
#define FREQUENTIST_ANALYTIC_ANALYSIS_DEBUGCHANNELREADER_H
#include "DebugChannel.h"
#include <initializer_list>
#include <vector>
// DebugChannelReader.h: A channel reader that can be used to inject debugging data.
// Look at BasicChannelReader.h for specifications on a ChannelReader.
class DebugChannelReader
{
	public:
		//Builds a DebugChannelReader from an initializer list.
		DebugChannelReader(std::initializer_list<DebugChannel> iDebugChannels);

		//Accesses a debugChannel from its initializer list
		const DebugChannel& operator[](int index) const;

		//Accesses how many channels the channel reader was initialized with.
		int getChannelsRead();

	private:
		std::vector<DebugChannel> debugChannels;
};
#endif
