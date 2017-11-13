cmake_minimum_required (VERSION 2.8.11)
project (tcp_chat)

file(GLOB_RECURSE COMMON_HEADER_FILES ./common/*.hpp)

file(GLOB_RECURSE CLIENT_HEADER_FILES ./client/*.hpp)
add_executable(client ./client/main.cpp ${COMMON_HEADER_FILES} ${CLIENT_HEADER_FILES})
target_link_libraries(client pthread)

file(GLOB_RECURSE SERVER_HEADER_FILES ./server/*.hpp)
add_executable(server ./server/main.cpp ${COMMON_HEADER_FILES} ${SERVER_HEADER_FILES})