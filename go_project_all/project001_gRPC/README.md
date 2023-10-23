1. 创建一个基于go的使用gRPC和protobuf通信的程序，用来传递一些需要用类型包裹的数据，实现一个远程计算器功能
2. 获取go的docker镜像，go pull golang
3. 创建docker镜像，将git项目目录映射到容器的/tmp/project001_grpc，`--security-opt seccomp=unconfined`配置项用于设定安全配置文件
docker run --rm -it -m 500M --memory-swap=1G -p 8082:80 -p 8083:8083 --security-opt seccomp=unconfined --name my_grpc -v /root/cpp_test/Practice_Code/go_project_all/project001_gRPC:/tmp/project001_grpc -d golang 
4. hello中存放服务端源码，proto中存放生成的pb.go文件
5. 使用grpc官网的安装流程，https://grpc.io/docs/languages/go/quickstart/
