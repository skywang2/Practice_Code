#pragma once
#include <string>
#include <memory>

//9.ʹ��pImpl��ʽ�����Ա����
class TestMemberData
{
public:
	TestMemberData(int data1);
	~TestMemberData();

private:
	class MemberDataImpl;
	std::unique_ptr<MemberDataImpl> pImpl;//unique_ptr�ڱ���׶���Ҫȷ����Ĵ�С
};
