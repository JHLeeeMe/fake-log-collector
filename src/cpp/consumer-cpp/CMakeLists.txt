cmake_minimum_required(VERSION 3.10)
project(consumer-cpp)

include_directories(/usr/include/librdkafka)
include_directories(include)

add_executable(consumer-cpp main.cpp consumer_configs.cpp consumer_utils.cpp mqueue_sender.cpp)
target_link_libraries(consumer-cpp PRIVATE rdkafka++ fmt)
