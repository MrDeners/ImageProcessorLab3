cmake_minimum_required(VERSION 3.0.0)
project(Lab3 VERSION 0.1.0)

add_executable(Practic main.c func.h func.c )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)