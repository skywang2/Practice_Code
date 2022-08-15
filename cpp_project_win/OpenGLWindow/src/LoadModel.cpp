#include "LoadModel.h"
#include <fstream>
#include <string>
#include <sstream>

bool LoadVertexPosition(const std::string& filepath, float* arr, unsigned int count)
{
	if (filepath.empty() || !arr)
	{
		return false;
	}
	memset(arr, 0, sizeof(float) * count);

	std::string line;
	std::fstream file(filepath, std::fstream::in);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			std::string one;
			std::stringstream ss(line);
			while (std::getline(ss, one, ','))
			{
				if (0 == count) { break; }
				*arr = atof(one.c_str());
				arr++;
				count--;				
			}
		}
		file.close();
		return true;
	}

	return false;
}
