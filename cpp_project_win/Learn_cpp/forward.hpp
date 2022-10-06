#pragma once
#include <string>
#include <iostream>

void TestProcess(const std::string& s)
{
	std::cout << "&" << std::endl;
}

void TestProcess(std::string&& s)
{
	std::cout << "&&" << std::endl;
}

//�������ã�ֻ��ģ�庯��������������auto&&��Ϊ��Σ������ʹ��
//��Ϊʵ����Ҫ�Ƶ����ͣ�����д����&&ʵ���Ͽ�����&��ֵ����
//���Ҫʹ��forward��param��ԭ����ȷ������
template<typename T>
void MoreThanProcess(T&& param)
{
	//std::cout << __FUNCTION__ << std::endl;
	
	std::cout << "without std::forward" << std::endl;
	TestProcess(param);
	std::cout << "with std::forward" << std::endl;
	TestProcess(std::forward<T>(param));
}