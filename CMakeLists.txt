cmake_minimum_required(VERSION 3.27)
project(native_uspeak_codecs)

set(CMAKE_CXX_STANDARD 17)

add_library(native_uspeak_codecs STATIC library.cpp
        src/codec.h
        src/codec/MuLawCodec.cpp
        src/codec/MuLawCodec.h
        src/codec/ADPCMCodec.cpp
        src/codec/ADPCMCodec.h)
