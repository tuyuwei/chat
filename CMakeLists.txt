cmake_minimum_required(VERSION 3.6)
project(chat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    #client/client.c
    #nbproject/private/c_standard_headers_indexer.c
    #nbproject/private/cpp_standard_headers_indexer.cpp
    server/ae.c
    server/ae.h
    server/ae_epoll.c
    server/anet.c
    server/anet.h
    server/chat.c
    server/chat.h
    server/setproctitle.c
    #server/test.c
    server/zmalloc.c
    server/zmalloc.h server/networking.cpp server/networking.h server/networking.c server/networking.h)

#add_custom_target()
add_executable(chat ${SOURCE_FILES})