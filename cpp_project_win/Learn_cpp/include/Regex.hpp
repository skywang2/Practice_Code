#ifndef REGEX_HPP
#define REGEX_HPP
#include <iostream>
#include <regex>
#include <string>
#include <vector>
using std::string;
using std::cout;
using std::endl;
using std::regex;
using std::vector;

typedef enum class _STRTYPE
{
	HTML = 0,
	EMAIL,

}STRTYPE;

string GetStrings(STRTYPE type)
{
	string s;
	switch (type)
	{
	case STRTYPE::HTML:
		s = "<p>Hello World</p>";
		break;
	case STRTYPE::EMAIL:
		s = "123@qq.vip.com,456@gmail.com,789@163.com,abcd@my.com";
		break;
	default:
		break;
	}

	return std::move(s);
}

//regex_match��ȫƥ�䣬ֻ�ж��Ƿ�ƥ��
void RegexMatch()
{
	cout << __FUNCTION__ << endl;
	string s = GetStrings(STRTYPE::HTML);

	regex reg("<.*>.*</.*>");
	bool ret01 = std::regex_match(s, reg);
	cout << "regex match 01:" << ret01 << endl;

	bool ret02 = std::regex_match(s, regex("<(.*)>.*</\\1>"));//�������ʽ��\1��ʾ���һ����������ͬ��\\��ʾת��\ 
	cout << "regex match 02:" << ret02 << endl;
}

//���ƥ����
void RegexMatchResult()
{
	cout << __FUNCTION__ << endl;
	string s = GetStrings(STRTYPE::HTML);
	std::smatch m;//smatch���string��cmatch���char*
	bool ret03 = std::regex_match(s, m, regex("<(.*)>(.*)</(.*)>"));//ʹ��\\1�ǲ������ŵģ����ᱣ��ƥ���ִ�
	if (ret03)
	{
		cout << "m.str:" << m.str() << endl;
		cout << "m.length:" << m.length() << endl;
		cout << "m.position:" << m.position() << endl;
		for (auto str : m)
		{
			cout << "m:" << str << endl;
		}
	}

}

//search������ƥ���ִ�������true
void RegexSearch()
{
	cout << __FUNCTION__ << endl;
	string s = GetStrings(STRTYPE::HTML);
	std::smatch m;//smatch���string��cmatch���char*
	bool ret04 = std::regex_search(s, m, regex("<.*>(.*)</.*>"));
	if (ret04)
	{
		cout << "prefix:" << m.prefix() << endl;
		cout << "suffix:" << m.suffix() << endl;
		for (auto str : m)
		{
			cout << "m:" << str << endl;
		}
	}
}

//tokenize���ִ�
void RegexTokenize()
{
	cout << __FUNCTION__ << endl;
	string s = GetStrings(STRTYPE::EMAIL);
	regex reg(",");//���ܰ�regex(",")ֱ�ӷŵ�iterator���캯����Σ���Ϊ������ֵ���õ�����ɾ�������غ���
	std::sregex_token_iterator end;
	std::sregex_token_iterator pos(s.begin(), s.end(), reg, -1);
	for (; pos != end; ++pos)
	{
		cout << pos->str() << endl;
	}
}

//�滻�ַ�
void RegexReplace()
{
	cout << __FUNCTION__ << endl;
	string s = "he..o wor.d";
	cout << std::regex_replace(s, regex("\\."), "l") << endl;
}

#endif // !REGEX_HPP