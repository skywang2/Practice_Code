#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "pugixml.hpp"

int main()
{
	std::string text;
	std::fstream file("TestFile.txt", std::fstream::in);

	try
	{
		if (file.is_open())
		{
			//file.seekg(0, file.end);
			//int length = file.tellg();
			//file.seekg(0, file.beg);

			//char* temp = new char[length];
			//memset(temp, 0, length);
			//file.read(temp, length);
			//text = temp;
			//delete[] temp;

			std::string line;
			std::stringstream ss;
			while (std::getline(file,line))
			{
				ss << line;
			}
			text = ss.str();
			file.close();
		}
		else
		{
			throw std::string("open file failed");
		}
		//解析XML
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file("TestFile.txt");//使用load_file可直接读取文件
		if (!result)
		{
			throw std::string(result.description());
		}
		pugi::xpath_node_set DscriptorList = doc.select_nodes("/DynamicCap/ResolutionAvailableDscriptorList/ResolutionAvailableDscriptor");
		for (auto it = DscriptorList.begin(); it != DscriptorList.end(); ++it)
		{
			const pugi::xml_node& Dscriptor = (*it).node();
			std::cout << Dscriptor.name() << std::endl;

			pugi::xml_node Width = Dscriptor.child("videoResolutionWidth");
			pugi::xml_node Height = Dscriptor.child("videoResolutionHeight");
			pugi::xml_node EncodeType = Dscriptor.child("supportedEncodeType");
			std::cout << Width.child_value() << std::endl;
			std::cout << Height.child_value() << std::endl;
			std::cout << EncodeType.child_value() << std::endl;
		}
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}

	system("pause");
	return 0;
}
