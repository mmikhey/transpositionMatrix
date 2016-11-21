// transposition.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <cassert>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>

using std::size_t;

void swap(std::vector< int >& transposition, int k, int i)
{
	int tmp = transposition[k];
	transposition[k] = transposition[i];
	transposition[i] = tmp;
}

void enumeration(std::vector< std::vector<int> > matrix, std::vector< int >& transposition, int k, int n, int& criterion, std::vector< int >& result_transposition)
{
	for (int i = k; i < n; i++)
	{
		swap(transposition, k, i);
		enumeration(matrix, transposition, k + 1, n, criterion, result_transposition);
		swap(transposition, k, i);
	}
	if (k == n - 1) 
	{
		/*for (int i = 0; i < transposition.size(); i++)
		{
			std::cout << transposition[i];
		}
		std::cout << "\n";*/
		int local_criterion = 0;
		for (int i = 0; i < matrix.size(); i++)
		{
			for (int j = 0; j < matrix.at(i).size(); j++)
			{
				local_criterion += matrix[i][j] * matrix[transposition[i]][transposition[j]];
			}
		}
		if (local_criterion>criterion)
		{
			criterion = local_criterion;
			for (int i = 0; i < transposition.size(); i++)
			{
				result_transposition[i] = transposition[i];
			}
		}
	}
}

int main(int ARGC, char** ARGV)
{
	int n=5;
	std::vector< std::vector<int> > matrix;
	matrix.resize(n);
	std::vector< int> transposition;
	transposition.resize(n);
	std::vector< int> result_transposition;
	result_transposition.resize(n);

	int criterion = 0;
		
		for (int i = 0; i < n; ++i)
		{
			srand(time(0) | clock());
			for (int j = 0; j < n; ++j)
			{
				int value= rand()%2;
				matrix.at(i).push_back(value);
				//srand(time(0)|clock());
			}
			
		}


		for (int i = 0; i < matrix.size(); ++i)
		{
			transposition[i]=i;
			result_transposition[i]=i;
		}
		int tmp = 0;
		enumeration(matrix, transposition, 0, n, criterion,result_transposition);
		
		std::cout <<"criterion = "<< criterion << "\n";
		for (int i = 0; i < transposition.size(); i++)
		{
		std::cout << result_transposition[i];
		}
		std::cout << "\n";

	return 0;
}

