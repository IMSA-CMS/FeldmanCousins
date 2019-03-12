// csvConverter.cpp: Takes a csv and converts it into usable format for the application.

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "csvConverter.h"


bool charIsNum(char in)
{
	bool out = false;
	if ('0' <= in && in <= '9')
	{
		out = true;
	}
	return out;
}



std::vector<std::string> oneLine(std::stringstream& in)
{
	std::string line;
	std::vector<std::string> out;
	while (std::getline(in, line, ','))
	{
		out.push_back(line);
	}
	return out;
}



void oneBin(std::stringstream& in, std::ofstream& outFile)
{
	std::vector<std::string> line = oneLine(in);
	outFile << "000000000000000\n";
	for (std::size_t i = 0; i < line.size(); ++i)
	{
		outFile << line.at(i) << "\n";
	}
}



void allBins(std::ifstream& inFile, std::ofstream& outFile)
{
	std::string line;
	std::stringstream temp;
	while (std::getline(inFile, line))
	{
		std::cout << line << std::endl;
		if (charIsNum(line.at(0)))
		{
			std::cout << "in" << std::endl;
			temp << line;
			temp.str(std::string());
			oneBin(temp, outFile);
		}
	}
	outFile << "000000000000000";
}

