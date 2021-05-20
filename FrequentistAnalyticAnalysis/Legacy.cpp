// You've reached Legacy.cpp, the place where all your old mains go!
// First, some rules to keep this place organized:
// 1. Name your old mains mainMMDDYYYY, eg. main07162020 
//    (for the date 7/16/2020, and yes, I know one day this date will seem comically old)
// 2. Leave a brief comment above the declaration about what the code means in the moment.
//	  You may feel this is unimportant, but it will help you keep track of your mains.
// 3. Keep these mains as up-to-date as reasonably possible, and when you feel that they have fully
//    obsolesced, feel free to discard them. This is not meant to be a permanent storage space
//    (that's what GitHub is for).


//Includes
#include <iostream>
#include <fstream>
#include "BasicChannelReader.h"
#include "SingleChannelPoissonDistribution.h"
#include "FirstAnalyticParticleGraph.h"
#include "NeighborIterator.h"
#include "SecondAnalyticParticleGraph.h"
#include "FourthAnalyticParticleGraph.h"
#include "ThirdAnalyticParticleGraph.h"
#include "FourthAnalyticParticleGraph.h"

//Contains the main for the singleChannelPoissonDistribution case (for the first Distribution/Graph class)
int main07162020()
{
    BasicChannelReader reader("bin1.txt");
    std::cout << std::boolalpha;
    constexpr double lower = 3, upper = 10, confidenceLevel = 0.95;
    constexpr int nSegments = 70;
    for (int i = 0; i < nSegments; ++i)
    {
        const double lambda = lower + (upper - lower) / nSegments * i;
        SingleChannelPoissonDistribution distribution(reader, 1 / (lambda * lambda));
        std::cout << "Lambda: " << lambda << "\nInside: "
            << distribution.checkConfidenceBand(confidenceLevel)
            << std::endl;
    }
    return 0;
}

//Contains the main for the FirstAnalyticParticleGraph.h case (for the first multidimensional class)
int main07212020()
{
    BasicChannelReader reader("bin12.txt");
    std::cout << std::boolalpha;
    constexpr double lower = 9, upper = 11, confidenceLevel = 0.95;
    constexpr int nSegments = 20;
    for (int i = 0; i < nSegments; ++i)
    {
        const double lambda = lower + (upper - lower) / nSegments * i;
        FirstAnalyticParticleGraph distribution(reader, 1 / (lambda * lambda));
        std::cout << "Lambda: " << lambda << "\nInside: "
            << distribution.checkConfidenceRegion(confidenceLevel)
            << std::endl;
    }
    return 0;
}
//Contains the main for testing NeighborIterator.h
int main07222020()
{
    Coordinate<int> c(2);
    c[0] = 5;
    c[1] = 5;
    for (NeighborIterator i(c); !i.isExhausted(); i.next())
    {
        auto current = i.getCurrent();
        std::cout << current[0] << ", " << current[1] << '\n';
    }
    return 0;
}

//Contains the main for the SecondAnalyticParticleGraph.h
int main07232020()
{
    BasicChannelReader reader("bin12.txt");
    std::cout << std::boolalpha;
    constexpr double lower = 9, upper = 11, confidenceLevel = 0.95;
    constexpr int nSegments = 20;
    for (int i = 0; i < nSegments; ++i)
    {
        const double lambda = lower + (upper - lower) / nSegments * i;
        SecondAnalyticParticleGraph distribution(reader, 1 / (lambda * lambda));
        std::cout << "Lambda: " << lambda << "\nInside: "
            << distribution.checkConfidenceRegion(confidenceLevel)
            << std::endl;
    }
    return 0;
}

//Contains the main for ThirdAnalyticParticleGraph.h
int main08012020()
{
    BasicChannelReader reader("bin12.txt");
    std::cout << std::boolalpha;
    constexpr double lower = 9, upper = 11, confidenceLevel = 0.95;
    constexpr int nSegments = 20;
    for (int i = 0; i < nSegments; ++i)
    {
        const double lambda = lower + (upper - lower) / nSegments * i;
        ThirdAnalyticParticleGraph distribution(reader, 1 / (lambda * lambda));
        std::cout << "Lambda: " << lambda << "\nInside: "
            << distribution.checkConfidenceRegion(confidenceLevel)
            << std::endl;
    }
    return 0;
}

//Don't ask about the gap in dates.
//The life lesson in my case is to git push early and often (when it works).

//Contains the main for FourthAnalyticParticleGraph.h.
int main091420()
{
    BasicChannelReader reader("bin13.txt");
    std::cout << std::boolalpha;
    constexpr double lower = 10, upper = 11, confidenceLevel = 0.95;
    constexpr int nSegments = 10;
    for (int i = 0; i < nSegments; ++i)
    {
        const double lambda = lower + (upper - lower) / nSegments * i;
        FourthAnalyticParticleGraph distribution(reader, 1 / (lambda * lambda));
        std::cout << "Lambda: " << lambda << "\nInside: "
            << distribution.checkConfidenceRegion(confidenceLevel)
            << std::endl;
    }
    return 0;
}