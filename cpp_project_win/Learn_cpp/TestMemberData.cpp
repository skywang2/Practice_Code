#include "TestMemberData.h"
#include "Queue.hpp"

//pImpl是为了使例如data1、data2（假设）这类定义在其他文件中的成员变量类型修改时减少引用对象的编译
class TestMemberData::MemberDataImpl//需要定义在类内部
{
public:
	explicit MemberDataImpl(int data1) : data1(data1) {}

public:
	int data1;
	MyQueue data2;//此处data2只是用默认构造进行初始化，因此不需要入参
};

TestMemberData::TestMemberData(int data1)
	: pImpl(std::make_unique<MemberDataImpl>(data1))
{
}

TestMemberData::~TestMemberData()
{

}
