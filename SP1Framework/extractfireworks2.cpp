#include "extractfirework.h"

string extractfireworks2(COORD spawn, int x, int y, int *level)
{
	string myfile;
	string row;
	string fwart2;

	ifstream file("fireworks2.txt");

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, row);
			fwart2 += row;
		}
		file.close();
	}
	return fwart2;
}