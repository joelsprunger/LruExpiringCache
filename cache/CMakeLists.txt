cmake_minimum_required(VERSION 3.24)
project(cache)

set(CMAKE_CXX_STANDARD 17)

add_executable(cache main.cpp Item.h Cache.h LruExpiringCache.cpp LruExpiringCache.h)
