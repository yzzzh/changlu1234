cmake_minimum_required(VERSION 3.7)
project(changlu)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES sources/main.cpp headers/CountingBloomFilter.h sources/CountingBloomFilter.cpp headers/DLeftCountingBloomFilter.h sources/DLeftCountingBloomFilter.cpp headers/Routerlist.h headers/TrieTree.h sources/TrieTree.cpp headers/BKDRHash.h headers/constant.h)
add_executable(changlu ${SOURCE_FILES})