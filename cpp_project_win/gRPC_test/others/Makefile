CC = g++
CXXFLAGS = -std=c++11
TARGET = libproto.a
SOURCE = $(wildcard *.cc) # 为了在patsubst函数参数中正确展开，需要使用wildcard
OBJS = $(patsubst %.cc, %.o, $(SOURCE)) # $(patsubst <pattern>,<replacement>,<text> ) ，把从SOURCE中匹配到的.cc替换成.o
INCLUDE = -I./
PROTO_FILE = *.pb.h *.pb.cc

all:$(TARGET)

# 生成.a目标文件
$(TARGET): proto $(OBJS)
	ar rcv $(TARGET) $(OBJS)

# protoc生成cc等文件
proto:
	protoc -I=./ --cpp_out=./ ./echo.proto

# g++编译生成.o文件
%.o: %.c
	$(CC) $(CXXFLAGS) $(INCLUDE) -o $@ -c $^                                                                                                                                                                        

.PHONY:clean

# $(wildcard $(PROTO_FILE))，这句话是把PROTO_FILE提前展开
clean:
	-rm -f *.o $(TARGET) $(PROTO_FILE) 
