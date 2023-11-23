#include "PersonTest.h"
#include <string>

#include "proto/person.pb.h"

void PersonTest::Test()
{
    using PeopleInfo::Person;
    using PeopleInfo::Color;
    
    Person m_person;
    // set value
    m_person.set_id(10);
    m_person.set_name("wwww");
    m_person.set_sex("a");
    m_person.set_age(100);
    m_person.mutable_addr()->set_num(123);
    m_person.mutable_addr()->set_addr("xihu");
    m_person.set_color(Color::Blue);

    // 序列化
    std::string output;
    m_person.SerializeToString(&output);
    std::cout << output << std::endl;

    // 反序列化
    Person tmpP;
    bool ret = tmpP.ParseFromString(output);
    std::cout << tmpP.id() << ", "
        << tmpP.name() << ", "
        << tmpP.sex() << ", "
        << tmpP.age() << ", " << std::endl;
    
    
    return;
}
