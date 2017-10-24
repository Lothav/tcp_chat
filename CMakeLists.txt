cmake_minimum_required (VERSION 2.8.11)
project (tcp_chat)
file(GLOB_RECURSE CORE_HEADER_FILES *.hpp)
add_executable(tcp_chat main.cpp ${CORE_HEADER_FILES})