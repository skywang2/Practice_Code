1.Tree1.cpp，二叉树的创建，写的比较简陋
2.Queue_cpp_built-in.cpp，尝试使用queue结构
3.Circular_Queue.cpp，尝试写循环队列
4.实现MyLocker，使用RAII方式管理mutex资源，例用shared_ptr实现计数引用，删除拷贝构造、复制构造使其无法被拷贝，另外析构时shared_ptr将释放内存改为解锁
5.返回局部变量的const引用，这是错误的，会产生未初始化的变量，得到0xcccccccc（-858993460）
6.使用STL中的正则表达式类
7.使用vector, emplace()/emplace_back()
8.使用static_assert
9.使用pImpl方式保存成员数据
10.验证std::forward对模板函数中万能引用的影响
11.计算vector长度
12.访问者模式
13.定义只能在堆上创建的类（析构函数设为私有），定义只能在栈上创建的类（new和delete设为私有，重载operate new/delete函数）
14.测试*a++的执行顺序
15.在vs自带的prompt使用dumpbin命令行工具生成PE的dump文件查看可执行文件的段落结构
16.使用cpp的随机数生成类
