cmake_minimum_required(VERSION 3.13)

project(NetSim)
set(PROJECT_ID NetSim)

set(SOURCE_FILES
        src/package.cpp
        src/helpers.cpp
        src/storage_types.cpp
        src/nodes.cpp
        src/factory.cpp
        )

set(SOURCES_FILES_TESTS
        test/test_package.cpp
        test/test_nodes.cpp
)

include(SetCommonConfig.cmake)
