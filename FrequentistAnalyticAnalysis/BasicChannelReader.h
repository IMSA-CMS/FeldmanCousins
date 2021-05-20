#ifndef FREQUENTIST_ANALYTIC_ANALYSIS_BASICCHANNELREADER_H
#define FREQUENTIST_ANALYTIC_ANALYSIS_BASICCHANNELREADER_H
#include <string>
#include <fstream>
#include "BasicChannel.h"
#include <vector>
//BasicChannelReader.h: A ChannelReader that reads based on the basic file format.
class BasicChannelReader
{
    class Channel;
    public:
        //Builds a BasicChannelReader using the name of the file where the channels are located.
        BasicChannelReader(const std::string& filename);
        
        //Gets the channel read sequentially at position index
        //Does some tricks to make sure that channels are not read unneccesarily
        const BasicChannel& operator[](int index);

        //Gets the total number of channels in the file
        int getChannelsRead();
        
    private:
        //Loads all of the channels in the file into the buffer
        void loadChannels();

        //Loads a given number of channels into the buffer
        void loadChannels(int numberOfChannels);


        //Check for end-of-file
        bool checkEOF();

        bool eof;
        std::ifstream file;
        std::vector<BasicChannel> channels;
};
#endif