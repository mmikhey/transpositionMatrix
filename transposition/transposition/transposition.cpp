// transposition.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <cassert>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using std::size_t;

#include <Windows.h>
std::vector<std::string> GetFilesNamesInDir(const std::string& folder)
{
	std::vector<std::string> names;
	char search_path[200];
	sprintf_s(search_path, "%s\\*.*", folder.c_str());
	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA(search_path, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFileA(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

void WriteToFile(int criterion,std::vector< std::vector<int> >& transposition_matrix, const std::string dir_name)
{
	std::string filename(dir_name);
	filename.append("\\result.txt");
	std::fstream file(filename);
	bool file_open;
	if (file)
	{
		file_open = true;
	}
	else 
	{
		file_open=false;
	}
	file << criterion;
	file << "\n";
	for (size_t i = 0; i < transposition_matrix.size(); ++i)
	{
		for (size_t j = 0; j < transposition_matrix.at(i).size(); ++j)
		{
			file << transposition_matrix[i][j];
			file << " ";
		}
		file << "\n";
	}

}

int TranspositionRow(int row1, int row2, std::vector< std::vector<int> >& matrix, std::vector< std::vector<int> >& transposition_matrix)
{
	int local_criterion = 0;
	for (size_t i = 0; i < matrix.size(); ++i)
	{
		for (size_t j = 0; j < matrix.at(i).size(); ++j)
		{
			transposition_matrix[i][j]=matrix[i][j];
		}
	}
	for (int i = 0; i < matrix.at(row1).size(); i++)
	{
		transposition_matrix[row1][i] = matrix[row2][i];
		transposition_matrix[row2][i] = matrix[row1][i];
	}
	for (size_t i = 0; i < matrix.size(); ++i)
	{
		for (size_t j = 0; j < matrix.at(i).size(); ++j)
		{
			local_criterion += transposition_matrix[i][j] * matrix[i][j];
		}
	}
	return local_criterion;
}

int TranspositionCol(int col1, int col2, std::vector< std::vector<int> >& matrix, std::vector< std::vector<int> >& transposition_matrix)
{
	int local_criterion = 0;
	for (int i = 0; i < matrix.at(0).size(); i++)
	{
		char tmp = transposition_matrix[i][col1];
		transposition_matrix[i][col1] = transposition_matrix[i][col2];
		transposition_matrix[i][col2] = tmp;
	}
	for (size_t i = 0; i < matrix.size(); ++i)
	{
		for (size_t j = 0; j < matrix.at(i).size(); ++j)
		{
			local_criterion += transposition_matrix[i][j] * matrix[i][j];
		}
	}
	return local_criterion;
}

int main(int ARGC, char** ARGV)
{
	int n;
	std::vector< std::vector<int> > matrix;
	std::vector< std::vector<int> > transposition_matrix;

	int criterion = 0;

	std::vector<std::string> files;
	const std::string dir_name(ARGV[1]);
	if (ARGC > 1)
		files = GetFilesNamesInDir(dir_name);
	else
		files = GetFilesNamesInDir("..\\..\\TestData");
	unsigned int number_of_examples = files.size();

	for (unsigned int i = 0; i < 1; ++i)
	{
		std::string filename(dir_name);
		filename.append("\\matrix.txt");
		std::fstream file(filename);

		file >> n;
		matrix.resize(n);
		transposition_matrix.resize(n);
		for (size_t i = 0; i < n && !file.eof(); ++i)
		{
			for (size_t j = 0; j < n && !file.eof(); ++j)
			{
				int value;
				file >> value;
				matrix.at(i).push_back(value);
			}
		}


		for (size_t i = 0; i < matrix.size(); ++i)
		{
			for (size_t j = 0; j < matrix.at(i).size(); ++j)
			{
				transposition_matrix.at(i).push_back(matrix[i][j]);
			}
		}

		for (int row1 = 0; row1 < n; row1++)
		{
			for (int row2 = row1; row2 < n; row2++)
			{
				if (row1 != row2)
				{
					int local_criterion = TranspositionRow(row1, row2, matrix, transposition_matrix);
				    for (int col1 = 0; col1 < n; col1++)
				    {
					    for (int col2 = col1; col2 < n; col2++)
					    {
							if (col1 != col2)
							{
								local_criterion = TranspositionCol(col1, col2, matrix, transposition_matrix);
								if (local_criterion>criterion)
								{
									criterion = local_criterion;
									WriteToFile(criterion, transposition_matrix, dir_name);
								}
								TranspositionCol(col2, col1, matrix, transposition_matrix);
							}
					    }
				    }
			    }
			}
		}
	}





	return 0;
}

