cmake_minimum_required(VERSION 3.10)
project(consumer-cpp)

# Fetch librdkafka
include(FetchContent)
FetchContent_Declare(
  librdkafka
  GIT_REPOSITORY https://github.com/confluentinc/librdkafka.git
  GIT_TAG        v1.9.2
)
FetchContent_MakeAvailable(librdkafka)

include_directories(${CMAKE_BINARY_DIR}/_deps/librdkafka-src/src-cpp)
include_directories(include)

# Add your source files
add_executable(consumer-cpp main.cpp consumer_config.cpp mqueue_sender.cpp)

# Link against librdkafka
target_link_libraries(consumer-cpp PRIVATE rdkafka++)
