CC = g++
CXXFLAGS = -std=c++11
TARGET = event.elf
SOURCE = $(wildcard *.cpp) # 为了在patsubst函数参数中正确展开，需要使用wildcard
OBJS = $(patsubst %.cpp, %.o, $(SOURCE)) # $(patsubst <pattern>,<replacement>,<text> ) ，把从SOURCE中匹配到的.cc替换成.o
INCLUDE = -I./

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -levent

# g++编译生成.o文件
%.o: %.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	-rm -f *.o $(TARGET)

.PHONY:clean
