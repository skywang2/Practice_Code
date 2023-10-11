set(SRC_LIST main.cpp)
set(CMAKE_CXX_STANDARD 11)

#在cmake命令过程中增加打印
message(STATUS "BINARY dir:" ${PROJECT_BINARY_DIR})
message(STATUS "SOURCE dir:" ${PROJECT_SOURCE_DIR})
message(STATUS "EXECUTABLE dir:" ${EXECUTABLE_OUTPUT_PATH})
message(STATUS "LIBRARY dir:" ${LIBRARY_OUTPUT_PATH})

# 指定输出可执行文件名和编译文件列表
add_executable(${PROJECT_NAME} ${SRC_LIST})

# 链接fastcgi库
# find_package(fcgi++)
target_link_libraries(${PROJECT_NAME} libfcgi.so libfcgi++.so)
