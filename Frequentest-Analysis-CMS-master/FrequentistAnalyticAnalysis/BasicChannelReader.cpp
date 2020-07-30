#include "BasicChannelReader.h"
#include <limits>
#include <iostream>
BasicChannelReader::BasicChannelReader(const std::string& filename) :
    file(filename),
    eof(false)
{
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
}

const BasicChannel& BasicChannelReader::operator[](int index)
{
    if (index < channels.size())
        return channels[index];
    loadChannels(index - channels.size() + 1);
    return channels[index];
}

int BasicChannelReader::getChannelsRead()
{
    loadChannels();
    return channels.size();
}

void BasicChannelReader::loadChannels()
{
    double iConstCoeff, iLinCoeff, iSqCoeff, iBgOne, iBgTwo;
    int iObserved;
    while (!checkEOF())
    {
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        file >> iConstCoeff >> iLinCoeff >> iSqCoeff >> iBgOne >> iBgTwo >> iObserved;
        if (!checkEOF()) file >> std::ws;
        channels.emplace_back(iConstCoeff, iLinCoeff, iSqCoeff, iBgOne, iBgTwo, iObserved);
    }
}

void BasicChannelReader::loadChannels(int numberOfChannels)
{
    double iConstCoeff, iLinCoeff, iSqCoeff, iBgOne, iBgTwo;
    int iObserved;
    for (int i = 0; i < numberOfChannels; ++i)
    {
        file >> iConstCoeff >> iLinCoeff >> iSqCoeff >> iBgOne >> iBgTwo >> iObserved;
        channels.emplace_back(iConstCoeff, iLinCoeff, iSqCoeff, iBgOne, iBgTwo, iObserved);
    }
}

bool BasicChannelReader::checkEOF()
{
    if (eof)
        return true;
    file.exceptions(std::ifstream::goodbit);
    eof = (file.peek() == EOF);
    file.clear();
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    return eof;
}
