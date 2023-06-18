#include "CSV_reader.h"
#include<fstream>
#include<iostream>
#include<algorithm>



std::vector<std::string> CSV_reader::ReadLine(std::string line)
{
	std::vector<std::string> result;
	//pos!=std::string::npos
	int pos = line.find(",");

	//pushback all line in vector
	while (pos != std::string::npos) {
		pos = line.find(",");
		result.push_back(line.substr(0, pos));
		line = line.substr(pos + 1, line.length() - pos);
	}
	return result;
}

void CSV_reader::Convert()
{
	for (int i = 0; i < row_count; i++)
	{
		for (int j = 0; j < columns_count; j++)
		{
			ConvertCell(&Cells[i][j]);
		}
	}
	if (error != "")
	{
		Error();
	}
}

std::vector<int> CSV_reader::CellCheck(std::string cell_number)
{
	std::vector<int> result;
	for (int i = 0; i < columns_count; i++)
	{
		int pos = cell_number.find(headers[i]);
		if (pos == -1) continue;
		std::string temp;
		pos = pos + headers[i].size();
		temp = cell_number.substr(pos, cell_number.size());
		if (IntCheck(temp))
		{
			result.push_back(stoi(temp)-1);
			result.push_back(i);
		}
	}
	return result;
}

bool CSV_reader::IntCheck(std::string str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if ((str[i] < '0') || (str[i] > '9'))
		{
			return false;
		}
	}
	return true;
}

void CSV_reader::ConvertCell(Cell *cell)
{
	if (cell->formula.size() > 0)
	{
		if (cell->formula[0] != '=')
		{
			//check every symbol in formula for being number
			if (!IntCheck(cell->formula))
			{
				cell->result = "err";
				error = "Error! Wrong formula.";
				return;
			}
			else
			{
				cell->result = cell->formula;
				return;
			}
		}
		else
		{
			std::vector<int> temp;
			char operation;
			if (cell->formula.find("*") != std::string::npos)
			{
				operation = '*';
				int x = (cell->formula.find("*"));
				temp.push_back(x);
			}
			if (cell->formula.find("/") != std::string::npos)
			{
				operation = '/';
				int x = (cell->formula.find("/"));
				temp.push_back(x);
			}
			if (cell->formula.find("%") != std::string::npos)
			{
				operation = '%';
				int x = (cell->formula.find("%"));
				temp.push_back(x);
			}
			if (cell->formula.find("+") != std::string::npos)
			{
				operation = '+';
				int x = (cell->formula.find("+"));
				temp.push_back(x);
			}
			if (cell->formula.find("-") != std::string::npos)
			{
				operation = '-';
				int x = (cell->formula.find("-"));
				temp.push_back(x);
			}
			int min = *std::min_element(temp.begin(), temp.end());
			std::string ARG1 = cell->formula.substr(1, min - 1);
			std::string ARG2 = cell->formula.substr(min + 1, cell->formula.size());
			std::vector<int> arg1 = CellCheck(ARG1);
			std::vector<int> arg2 = CellCheck(ARG2);
			if ((arg1.size() == 0) || (arg2.size() == 0))
			{
				cell->result = "err";
				error = "Error! Wrong formula.";
				return;
			}
			ConvertCell(&Cells[arg1[0]][arg1[1]]);
			ConvertCell(&Cells[arg2[0]][arg2[1]]);
			ARG1 = Cells[arg1[0]][arg1[1]].result;
			ARG2 = Cells[arg2[0]][arg2[1]].result;
			if (!(ARG1.compare("err")) || !(ARG2.compare("err")))
			{
				cell->result = "err";
				return;
			}
			switch (operation)
			{
				case '*':
					cell->result = std::to_string(stoi(ARG1) * stoi(ARG2));
					break;
				case '/':
					if (stoi(ARG2) != 0)
					{
						cell->result = std::to_string(stoi(ARG1) / stoi(ARG2));
					}
					else
					{
						cell->result = "err";
						error = "Error! Wrong formula.";
					}
					break;
				case '+':
					cell->result = std::to_string(stoi(ARG1) + stoi(ARG2));
					break;
				case '-':
					cell->result = std::to_string(stoi(ARG1) - stoi(ARG2));
					break;
				case '%':
					if (stoi(ARG2) != 0)
					{
						cell->result = std::to_string(stoi(ARG1) % stoi(ARG2));
					}
					else
					{
						cell->result = "err";
						error = "Error! Wrong formula.";
					}
					break;
			}
		}
	}
	else
	{
		cell->result = "err";
		error = "Error! Empty formula.";
		return;
	}
}

void CSV_reader::ReadFile(std::string fileName)
{
	inputFile = fileName;
	std::ifstream input(inputFile);
	std::string line;

	if (input.is_open())
	{
		std::getline(input, line);
		headers=ReadLine(line);
		columns_count = headers.size();

		while (std::getline(input, line))
		{
			std::vector<std::string> read;;
			std::vector<Cell> temp;

			read = ReadLine(line);
			if(read.size()>columns_count)
			{
				error = "Error! Too many columns.";
				Error();
				return;
			}

			for (int i = 0; i < read.size();i++)
			{
				Cell temp2;
				temp2.formula = read[i];
				temp.push_back(temp2);
			}
			Cells.push_back(temp);
		}
		row_count = Cells.size();
	}
	else
	{
		error = "Error! File does not exist.";
		Error();
		return;
	}
	if (headers.size() == 0)
	{
		error = "Error! No headers.";
		Error();
		return;
	}
	if (headers.size() > 1)
	{
		for (int i = 0; i < headers.size()-1; i++)
		{
			for (int j = i+1; j < headers.size(); j++)
			{
				if (!(headers[i].compare(headers[j])))
				{
					error = "Error! File have same headers.";
					Error();
					return;
				}
			}
		}
	}
	Convert();
}

void CSV_reader::Show()
{
	//if (!((error == "")||(error=="Error! Wrong formula."))) return;
	//headers output
	for (int i = 0; i < headers.size(); i++)
	{
		std::cout << headers[i] << ",";
	}
	std::cout << std::endl;
	for (int i = 0; i<row_count; i++)
	{
		for (int j = 0; j < columns_count; j++)
		{
			std::cout<<Cells[i][j].result<<',';
		}
		std::cout << std::endl;
	}
}

void CSV_reader::Error()
{
	if (error != "") std::cout <<std::endl<< error << std::endl;
}
