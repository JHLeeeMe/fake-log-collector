cmake_minimum_required(VERSION 3.10)
project(save-to-hdfs)

include_directories(include)
#link_directories(lib)

# Add your source files
add_executable(save-to-hdfs main.cpp)

# Link against librdkafka
target_link_libraries(save-to-hdfs PRIVATE hdfs)
