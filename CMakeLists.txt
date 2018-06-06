cmake_minimum_required(VERSION 3.5.1)
project(snake)

set(CMAKE_CXX_STANDARD 11)

include_directories(zmq-helper)

set(SERVER_SOURCE_FILES
        Config.h
        Point.h
        Server.cpp
        Snake.cpp
        Snake.h
        SnakeServer.cpp
        SnakeServer.h
        World.cpp
        World.h)

set(CLIENT_SOURCE_FILES
        Client.cpp
        SnakeClient.cpp
        SnakeClient.h)

add_executable(client ${CLIENT_SOURCE_FILES})
add_executable(server ${SERVER_SOURCE_FILES})

target_link_libraries(server zmq pthread)
target_link_libraries(client zmq pthread)