cmake_minimum_required(VERSION 3.29)
project(lftc)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lftc
    .idea/.gitignore
    .idea/editor.xml
    .idea/lftc.iml
    .idea/misc.xml
    .idea/modules.xml
    .idea/vcs.xml
    .idea/workspace.xml
        main.cpp
        bst/BST.h
        bst/BST.cpp
        AF/AutomatFinit.cpp
)
