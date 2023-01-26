cmake_minimum_required(VERSION 3.10)
project(write-to-influxdb)

include_directories(include)
link_directories(lib)

add_executable(write-to-influxdb main.cpp mqueue_receiver.cpp influxdb_writer.cpp)
target_link_libraries(write-to-influxdb fmt)
