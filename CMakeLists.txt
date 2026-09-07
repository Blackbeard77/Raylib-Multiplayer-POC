cmake_minimum_required(VERSION 3.20)

project(RaylibMultiplayerPOC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Raylib provides rendering, input and window management for the client.
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)

# ENet provides the UDP-based networking layer shared by client and server.
FetchContent_Declare(
        enet
        GIT_REPOSITORY https://github.com/lsalzman/enet.git
        GIT_TAG master
)

FetchContent_MakeAvailable(raylib enet)

add_executable(MultiplayerClient
        client/main.cpp
        client/ClientNetwork.cpp
)

add_executable(MultiplayerServer
        server/main.cpp
        server/ServerNetwork.cpp
        server/ServerNetwork.h
)

target_include_directories(MultiplayerClient PRIVATE
        ${enet_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/shared
)

target_include_directories(MultiplayerServer PRIVATE
        ${enet_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/shared
)

target_link_libraries(MultiplayerClient PRIVATE raylib enet)
target_link_libraries(MultiplayerServer PRIVATE enet)