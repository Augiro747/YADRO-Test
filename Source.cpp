#include "CSV_reader.h"
#include<iostream>

int main(int argc, char* argv[])
{
	std::string fileName;
	if (argc > 1)
	{
		fileName = argv[1];
		CSV_reader csv;
		csv.ReadFile(fileName);
		csv.Show();
	}
	else
	{
		std::cout << "Error! Bad argument.";
	}
	return 0;
}