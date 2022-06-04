cmake_minimum_required(VERSION 3.10)

PROJECT(async_test)

ADD_SUBDIRECTORY(src bin)

INSTALL(FILES COPYRIGHT README DESTINATION share/doc/async_test_cmake/)
INSTALL(PROGRAMS runAsync.sh DESTINATION bin/)
INSTALL(DIRECTORY doc DESTINATION share/doc/async_test_cmake/)

