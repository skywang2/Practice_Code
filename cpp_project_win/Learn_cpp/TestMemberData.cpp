#include "TestMemberData.h"
#include "Queue.hpp"

//pImpl��Ϊ��ʹ����data1��data2�����裩���ඨ���������ļ��еĳ�Ա���������޸�ʱ�������ö���ı���
class TestMemberData::MemberDataImpl//��Ҫ���������ڲ�
{
public:
	explicit MemberDataImpl(int data1) : data1(data1) {}

public:
	int data1;
	MyQueue data2;//�˴�data2ֻ����Ĭ�Ϲ�����г�ʼ������˲���Ҫ���
};

TestMemberData::TestMemberData(int data1)
	: pImpl(std::make_unique<MemberDataImpl>(data1))
{
}

TestMemberData::~TestMemberData()
{

}