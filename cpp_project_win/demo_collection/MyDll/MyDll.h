#ifndef MYDLL_H
#define MYDLL_H

#ifdef MYDLL_EXPORT
_declspec(dllexport) void MyDllFunc1();
#else
_declspec(dllimport) void MyDllFunc1();
#endif

void MyDllFunc2();//不使用_declspec关键字无法正确导出函数，dll中将不存在该函数，在调用处会链接失败，无法解析的外部符号

#endif
