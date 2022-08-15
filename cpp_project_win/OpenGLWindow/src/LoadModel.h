#pragma once
#include <iostream>

/*brief	读取顶点坐标
param[in]	filepath，文件路径
param[out]	arr，数组首地址
param[in]	数组长度*/
bool LoadVertexPosition(const std::string& filepath, float* arr, unsigned int count);
