// FrequentistAnalyticAnalysis.cpp : This file contains the 'main' function. Program execution begins and ends there.
// (Programmer's note: resist the temptation to put anything other than the main function in here, and keep
// main under 20 lines! If your main is getting too long due to old (legacy) code, please put your
// old mains in Legacy.cpp with a proper timestamp and comment! If your main is getting too long
// for any other reason, use a class! Please write good code and comment it in all of the places I do;
// use lowerCamelCase for local variables, class members, and methods, and UpperCamelCase for your classes;
// and never use snake case! Be responsible, and always remember that there will be others who will be responsible
// with dealing with your code, so you don't want to make their job too difficult! 
// The code is yours now. -- Ari Fishkin, original creator of this code)

#include <iostream>
#include "BasicChannelReader.h"
#include "ThirdAnalyticParticleGraph.h"
#include "FourthAnalyticParticleGraph.h"
int main()
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
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
