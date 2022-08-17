#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/*brief	读取顶点属性，坐标、索引、纹理坐标
param[in]	filepath，文件路径
param[out]	arr，数组首地址
param[in]	数组长度*/
template<typename T>
bool LoadVertexAttri(const std::string& filepath, T* arr, unsigned int count)
{
	if (filepath.empty() || !arr)
	{
		return false;
	}
	memset(arr, 0, sizeof(T) * count);

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
				*arr = static_cast<T>(std::stof(one));
				arr++;
				count--;
			}
		}
		file.close();
		return true;
	}

	return false;
}
