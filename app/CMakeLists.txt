cmake_minimum_required(VERSION 3.4.1)

add_subdirectory(src/main/cpp/simplecv)

add_library(native-lib SHARED src/main/cpp/native-lib.cpp)

target_link_libraries(native-lib log SimpleCV)
