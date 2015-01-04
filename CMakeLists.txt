project(modular_bms)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
include_directories("${PROJECT_BINARY_DIR}")
set(SRCS /home/alrekr/Dropbox/Workspaces/qt/modular_bms/common.h})
add_executable(${PROJECT_NAME} ${SRC_LIST})

