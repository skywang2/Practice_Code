#include "CommodityTest.h"
#include <string>
#include <vector>
#include <QTest>

using std::string;
using std::vector;

//待测试函数字符串分割，根据子串分割，若首字符为分隔符，结果第一项是空字符串
bool MySplit(const string& str, vector<string>& result, const string& delim = " "){
	if (str.empty() || delim.empty())
	{
		return false;
	}
	result.clear();

	size_t startPos = 0;
	while (startPos < str.length())
	{
		size_t matchPos = str.find(delim, startPos);
		if (string::npos == matchPos)
		{
			result.push_back(str.substr(startPos));
			break;
		}
		result.push_back(str.substr(startPos, matchPos - startPos));
		startPos = matchPos + delim.length();
	}

	return true;
}

CommodityTest::CommodityTest(QObject* parent)
	:QObject(parent)
{
}


CommodityTest::~CommodityTest()
{
}

void CommodityTest::Case1_MySplit()
{
	string str("\n--123\n--456\n--789");
	string delim("\n--");
	vector<string> subs;
	bool ret = MySplit(str, subs, delim);
	//MySplit("123\n--456\n--789", subs, delim);
	//MySplit("123\n--\n--456\n--789", subs, delim);

	QVERIFY(ret == true);
}

TestClassB::TestClassB(QObject* parent)
	:QObject(parent)
{
}


TestClassB::~TestClassB()
{
}

void TestClassB::Case1_MySplit()
{
	string str("\n--123\n--456\n--789");
	string delim("\n--");
	vector<string> subs;
	bool ret = MySplit(str, subs, delim);
	//MySplit("123\n--456\n--789", subs, delim);
	//MySplit("123\n--\n--456\n--789", subs, delim);

	QVERIFY(ret == true);
}
