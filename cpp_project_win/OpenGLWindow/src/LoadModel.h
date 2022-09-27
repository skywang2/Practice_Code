#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/*brief	��ȡ�������ԣ����ꡢ��������������
param[in]	filepath���ļ�·��
param[out]	arr�������׵�ַ
param[in]	���鳤��*/
template<typename T>
unsigned int LoadVertexAttri(const std::string& filepath, T* arr, unsigned int count)
{
	if (filepath.empty() || !arr)
	{
		return 0;
	}
	memset(arr, 0, sizeof(T) * count);

	unsigned int lineNum = 0;
	std::string line;
	std::fstream file(filepath, std::fstream::in);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			if (line.find("//") != std::string::npos)
			{
				continue;
			}

			std::string one;
			std::stringstream ss(line);
			while (std::getline(ss, one, ','))
			{
				if (0 == count) { break; }
				*arr = static_cast<T>(std::stof(one));
				arr++;
				count--;
			}
			lineNum++;
		}
		file.close();
		return lineNum;
	}

	return lineNum;
}