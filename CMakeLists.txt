cmake_minimum_required(VERSION 3.4.1)
project(file)

set(CMAKE_CXX_STANDARD 17)

include_directories("bookmodel")
include_directories("file")
include_directories("unix")
include_directories("read")
include_directories("encoding")
include_directories("encoding/util")
include_directories("util")

add_library(file-lib SHARED
        file/ZLFile.cpp
        file/ZLFSManager.cpp
        util/ZLFileUtil.cpp
        util/shared_ptr.h
        util/ZLStringUtil.cpp
        unix/ZLUnixFileInputStream.cpp
        unix/ZLUnixFileOutputStream.cpp
        unix/ZLUnixFSManager.cpp

        bookmodel/ZLCachedMemoryAllocator.cpp
        bookmodel/ZLTextModel.cpp

        encoding/DummyEncodingConverter.cpp
        encoding/Utf8EncodingConverter.cpp
        encoding/Utf16EncodingConverters.cpp
        encoding/ZLEncodingCollection.cpp
        encoding/ZLEncodingConverter.cpp
        encoding/util/ZLUnicodeUtil.cpp

        read/BookReader.cpp
        read/EncodedTextReader.cpp
        read/PlainTextFormat.cpp
        read/TxtBookReader.cpp
        read/TxtReader.cpp)

add_executable(file main.cpp
        file/ZLFile.cpp
        file/ZLFSManager.cpp
        unix/ZLUnixFileInputStream.cpp
        unix/ZLUnixFileOutputStream.cpp
        unix/ZLUnixFSManager.cpp
        util/ZLStringUtil.cpp
        util/ZLFileUtil.cpp
        util/shared_ptr.h

        file/ZLFile.cpp
        file/ZLFSManager.cpp
        util/ZLFileUtil.cpp
        util/shared_ptr.h
        util/ZLStringUtil.cpp
        unix/ZLUnixFileInputStream.cpp
        unix/ZLUnixFileOutputStream.cpp
        unix/ZLUnixFSManager.cpp

        bookmodel/ZLCachedMemoryAllocator.cpp
        bookmodel/ZLTextModel.cpp

        encoding/DummyEncodingConverter.cpp
        encoding/Utf8EncodingConverter.cpp
        encoding/Utf16EncodingConverters.cpp
        encoding/ZLEncodingCollection.cpp
        encoding/ZLEncodingConverter.cpp
        encoding/util/ZLUnicodeUtil.cpp

        read/BookReader.cpp
        read/EncodedTextReader.cpp
        read/PlainTextFormat.cpp
        read/TxtBookReader.cpp
        read/TxtReader.cpp)