set(CMAKE_CXX_STANDARD 11)
set(BIN_NAME ${PROJECT_NAME}_test)

message(STATUS "PROJECT_SOURCE_DIR dir:" ${PROJECT_SOURCE_DIR})

# 头文件目录
include_directories(
    ${PROJECT_SOURCE_DIR}/src/proto
)

# 找依赖包
find_package(Protobuf REQUIRED)
if(Protobuf_FOUND)
    message(STATUS "PROTOBUF_LIBRARIES dir:" ${PROTOBUF_LIBRARIES})
else()
    message(STATUS  "Protobuf package not found.")
endif()

add_executable(${BIN_NAME})
file(GLOB sources CONFIGURE_DEPENDS *.cpp proto/*.cc)
target_sources(${BIN_NAME} PRIVATE ${sources})
target_link_libraries(${BIN_NAME} ${PROTOBUF_LIBRARIES})
