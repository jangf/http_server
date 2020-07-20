cmake_minimum_required(VERSION 3.0)
project(http_server)
#SET(CMAKE_CXX_FLAGS -pthread)
 #在编译选项中加入c++11支持
 add_compile_options(
	-std=c++11
) 
include_directories(
	./include
	./include/wrapper
	./include/debug
)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)#指定可执行文件路径
set(SRC ./src/wrapper/mutex.cpp ./src/wrapper/socket.cpp ./src/http_gram.cpp  ./src/reactor.cpp ./src/multi_thread.cpp ./src/main.cpp)
link_libraries(pthread)
add_executable(example ${SRC}) 