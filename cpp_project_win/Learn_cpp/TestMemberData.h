#pragma once
#include <string>
#include <memory>

//9.使用pImpl方式保存成员数据
class TestMemberData
{
public:
	TestMemberData(int data1);
	~TestMemberData();

private:
	class MemberDataImpl;
	std::unique_ptr<MemberDataImpl> pImpl;//unique_ptr在编译阶段需要确定类的大小
};

