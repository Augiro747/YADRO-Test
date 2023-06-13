#include<string>
#include<vector>
#pragma once


class Cell
{
public:
	std::string formula="";
	std::string result="";
};

class CSV_reader
{
	std::vector<std::vector<Cell>> Cells;
	std::vector<std::string> headers;
	std::string inputFile;
	std::string error="";
	int columns_count;
	int row_count;

	std::vector<std::string> ReadLine(std::string line);
	std::vector<int> CellCheck(std::string);
	bool IntCheck(std::string str);
	void ConvertCell(Cell *cell);
	void Convert();
	void Error();

public:
	void ReadFile(std::string fileName);
	void Show();
};

