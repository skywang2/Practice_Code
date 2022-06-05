1.使用CMake构建一个完整的项目结构，包含源码目录src、构建目录build、文档目录doc，以及一些其他文件如COPYRIGHT、README.txt
2.使用INSTALL命令安排make install步骤所需安装动作（由于系统权限原因，未实测）
3.需要手动新建build文件夹，并在其中进行外部构建，使得临时文件都存储与build文件夹
4.该项目尝试使用stl的异步线程函数
