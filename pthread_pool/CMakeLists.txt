cmake_minimum_required(VERSION 2.8)
project(DEMO)
aux_source_directory(. DIR_SRCS)
add_executable(DEMO ${DIR_SRCS})
