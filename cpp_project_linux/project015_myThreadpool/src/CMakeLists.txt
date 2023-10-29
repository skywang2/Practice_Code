set(CMAKE_CXX_STANDARD 11)
set(SRC main.cpp ThreadPool.cpp)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} Threads::Threads)
