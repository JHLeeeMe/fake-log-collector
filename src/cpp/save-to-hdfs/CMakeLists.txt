cmake_minimum_required(VERSION 3.10)
project(save-to-hdfs)

include_directories(include)
link_directories(lib)

add_executable(save-to-hdfs main.cpp mqueue_receiver.cpp hdfs_writer.cpp date_sync.cpp)
target_link_libraries(save-to-hdfs curl fmt)
