set(SRC mylib.cpp)
# 如果没有在命令行指定，则使用代码中指定的值
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# 编译一个静态库文件
add_library(myHelloLib STATIC ${SRC})

message(STATUS "PROJECT_SOURCE_DIR dir:" ${PROJECT_SOURCE_DIR})
message(STATUS "CURRENT_SOURCE_DIR dir:" ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "CURRENT_LIST_DIR dir:" ${CMAKE_CURRENT_LIST_DIR})
message(STATUS "CURRENT_BINARY_DIR dir:" ${CMAKE_CURRENT_BINARY_DIR})

# 指定目标的包含路径
target_include_directories(myHelloLib 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}># 给库开发者用的头文件路径
    PUBLIC $<INSTALL_INTERFACE:include>) # 给库调用者用的路径，相对于CMAKE_INSTALL_PREFIX的路径，影响使用find_package的程序的查找路径

# 用属性指定要安装的头文件
set_target_properties(myHelloLib PROPERTIES 
    PUBLIC_HEADER "mylib.h")

# 指定安装目标、导出对象、安装路径
install(TARGETS myHelloLib
    EXPORT myHelloLibTarget #导出的名字
    PUBLIC_HEADER DESTINATION include # 头文件安装路径
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

# 指定导出对象的一些属性
install(EXPORT myHelloLibTarget
    NAMESPACE mylib::
    FILE myHelloLibTarget.cmake DESTINATION lib/cmake/mylib) # 该文件定义了导出目标及相关信息

# 使用.cmake.in文件生成.cmake配置文件，添加导出配置
include(CMakePackageConfigHelpers)
configure_package_config_file(
    myHelloLibConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/myHelloLibConfig.cmake # config文件生成路径
    INSTALL_DESTINATION lib/cmake) # config文件安装路径

# 安装.cmake配置文件
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/myHelloLibConfig.cmake DESTINATION lib/cmake/mylib)
    