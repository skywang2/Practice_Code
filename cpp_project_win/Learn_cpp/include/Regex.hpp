#ifndef REGEX_HPP
#define REGEX_HPP
#include <iostream>
#include <regex>
#include <string>
using std::string;
using std::cout;
using std::endl;
using std::regex;

typedef enum class _STRTYPE
{
	HTML = 0,

}STRTYPE;

string GetStrings(STRTYPE type)
{
	string s;
	switch (type)
	{
	case STRTYPE::HTML:
		s = "<p>Hello World</p>";
	default:
		break;
	}

	return std::move(s);
}

//regex_matchÍêÈ«Æ¥Åä
void RegexMatch()
{
	string s = GetStrings(STRTYPE::HTML);
	regex reg("<.*>.*</.*>");
	bool ret = std::regex_match(s, reg);
}

#endif // !REGEX_HPP
