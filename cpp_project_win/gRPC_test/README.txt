1. 创建一个用于打通gRPC通信的客户端项目，服务端对应于go_project_all/project001_gRPC项目
2. 该项目使用vcpkg管理gRPC库，并联动vs的NuGet包管理器，联动方式参考：https://www.cnblogs.com/linuxAndMcu/p/14696542.html
3. ./vcpkg.exe integrate project，在当前目录生成一个NuGet配制文件，在vs -> 工具 -> NuGet 包管理器 -> 程序包管理器设置，新建一个源，目录填<vcpkg_dir>\scripts\buildsystems
4. 在具体某个项目上右键点管理NuGet程序包，安装找到的vcpkg包
5. 如果上述包找不到，可以在程序包管理控制台输入生成NuGet配制文件时给的命令，Install-Package "vcpkg.E.ccppcode.vcpkg" -Source "E:\c_cpp_code\vcpkg"
6. NuGet记录的包信息在cpp_project_win\packages中
7. win项目中链接器-输入-附加依赖项中的lib条目可能需要自己手动填写，根据vcpkg安装的lib来填，有很多，分不清的话把lib全天上，NuGet包管理器会在编译后把需要的dll自动拷贝到exe输出目录，打包只需要用到exe和dll即可
