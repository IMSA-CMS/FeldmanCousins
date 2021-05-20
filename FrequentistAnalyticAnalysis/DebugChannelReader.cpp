#include "DebugChannelReader.h"

DebugChannelReader::DebugChannelReader(std::initializer_list<DebugChannel> iDebugChannels)
    : debugChannels(iDebugChannels)
{
}

const DebugChannel& DebugChannelReader::operator[](int index) const
{
    return debugChannels.at(index);
}

int DebugChannelReader::getChannelsRead()
{
    return debugChannels.size();
}
