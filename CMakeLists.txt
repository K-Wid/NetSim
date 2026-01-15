cmake_minimum_required(VERSION 3.13)

project(NetSim)
set(PROJECT_ID NetSim)

set(SOURCE_FILES
        src/package.cpp
        src/helpers.cpp
        src/storage_types.cpp
        )

set(SOURCES_FILES_TESTS
        test/test_package.cpp
)

include(SetCommonConfig.cmake)
