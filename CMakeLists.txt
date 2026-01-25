cmake_minimum_required(VERSION 3.13)

set(PROJECT_ID NetSim)

message(STATUS "PROJECT: ${PROJECT_ID}")
project(${PROJECT_ID})

# Standard języka C++
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic -pedantic-errors -Wconversion)

include_directories(include)

set(SOURCE_FILES
        src/package.cpp
        src/helpers.cpp
        src/storage_types.cpp
        src/nodes.cpp
        # Dołożenie niedokończonego pliku skutkuje brakiem możliwości testowania
        # src/factory.cpp
        )

set(EXEC_DEBUG ${PROJECT_ID}__debug)
add_executable(${EXEC_DEBUG} ${SOURCE_FILES} main.cpp)

include_directories(mocks)
set(EXEC_TEST ${PROJECT_ID}__test)
set(GTEST_ROOT ../googletest-master)
add_subdirectory(${GTEST_ROOT} googletest-master)

# test_package
add_executable(${EXEC_TEST}_package ${SOURCE_FILES} test/test_package.cpp test/test_storage_types.cpp test/main_gtest.cpp)
target_compile_definitions(${EXEC_TEST}_package PUBLIC EXERCISE_ID=EXERCISE_ID_PACKAGES)
target_include_directories(${EXEC_TEST}_package PUBLIC ${GTEST_ROOT}/googlemock/include ${GTEST_ROOT}/googletest/include)
target_link_libraries(${EXEC_TEST}_package gmock)

# test_nodes
add_executable(${EXEC_TEST}_nodes ${SOURCE_FILES} test/test_nodes.cpp test/main_gtest.cpp)
target_compile_definitions(${EXEC_TEST}_nodes PUBLIC EXERCISE_ID=EXERCISE_ID_NODES)
target_include_directories(${EXEC_TEST}_nodes PUBLIC ${GTEST_ROOT}/googlemock/include ${GTEST_ROOT}/googletest/include)
target_link_libraries(${EXEC_TEST}_nodes gmock)

# test_factory
add_executable(${EXEC_TEST}_factory ${SOURCE_FILES} test/test_Factory.cpp test/test_factory_io.cpp test/main_gtest.cpp)
target_compile_definitions(${EXEC_TEST}_factory PUBLIC EXERCISE_ID=EXERCISE_ID_FACTORY)
target_include_directories(${EXEC_TEST}_factory PUBLIC ${GTEST_ROOT}/googlemock/include ${GTEST_ROOT}/googletest/include)
target_link_libraries(${EXEC_TEST}_factory gmock)

# test_reporting
add_executable(${EXEC_TEST}_reporting ${SOURCE_FILES} test/test_reports.cpp test/main_gtest.cpp)
target_compile_definitions(${EXEC_TEST}_reporting PUBLIC EXERCISE_ID=EXERCISE_ID_REPORTING)
target_include_directories(${EXEC_TEST}_reporting PUBLIC ${GTEST_ROOT}/googlemock/include ${GTEST_ROOT}/googletest/include)
target_link_libraries(${EXEC_TEST}_reporting gmock)

# test_simulation
add_executable(${EXEC_TEST}_simulation ${SOURCE_FILES} test/test_simulate.cpp test/main_gtest.cpp)
target_compile_definitions(${EXEC_TEST}_simulation PUBLIC EXERCISE_ID=EXERCISE_ID_SIMULATION)
target_include_directories(${EXEC_TEST}_simulation PUBLIC ${GTEST_ROOT}/googlemock/include ${GTEST_ROOT}/googletest/include)
target_link_libraries(${EXEC_TEST}_simulation gmock)
