cmake_minimum_required(VERSION 3.0)

set(SRC_LIST main.cpp)
set(CMAKE_CXX_STANDARD 11)
set(myHelloLib_DIR /usr/lib/cmake/mylib)# 需要手动定义xxx_DIR路径，使得cmake能够找到xxxConfig.cmake文件
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

project(use_lib)

message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

find_package(myHelloLib REQUIRED)
if(myHelloLib_FOUND) # 改变量由cmake自动定义 
    message(STATUS "myHelloLib found")
else()
    message(STATUS "myHelloLib not found")
endif()

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} mylib::myHelloLib)
