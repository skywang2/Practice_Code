cmake_minimum_required(VERSION 3.0)

set( CMAKE_CXX_COMPILER "D:/mingw64/bin/g++.exe" )
set( CMAKE_C_COMPILER "D:/mingw64/bin/gcc.exe" )

PROJECT(code_test VERSION 0.1.0)

ADD_SUBDIRECTORY(code)
ADD_SUBDIRECTORY(test)
