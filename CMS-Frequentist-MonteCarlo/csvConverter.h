//TAKES A CSV AND CONVERTS IT INTO A USABLE FILE FORMAT
#ifndef CMS_FREQUENTIST_MONTECARLO_CSVCONVERTER_H
#define CMS_FREQUENTIST_MONTECARLO_CSVCONVERTER_H
#include <vector>
#include <sstream>

// Checks if a character is a number
bool charIsNum(char in);

// Writes one lines of the csv into a string
std::vector<std::string> oneLine(std::stringstream& file);

// Writes one bin into the file
void oneBin(std::stringstream& in, std::ofstream& outFile);

// Writes the entire CSV file into a text document
void allBins(std::ifstream& inFile, std::ofstream& outFile);
#endif
