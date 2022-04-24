cmake_minimum_required(VERSION 3.0.0)
project(iqToSharp VERSION 1.1)

add_executable(iqToSharp iqToSharp.cpp)
target_link_libraries(iqToSharp -static-libgcc -static-libstdc++)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
