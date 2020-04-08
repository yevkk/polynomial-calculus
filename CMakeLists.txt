cmake_minimum_required(VERSION 3.12)
project(lab)

set(CMAKE_CXX_STANDARD 17)

set(TOP_DIR ${CMAKE_SOURCE_DIR})
set(SRC_DIR ${TOP_DIR}/src)
set(TESTS_DIR ${TOP_DIR}/tests)
set(CMAKE_BUILD_TYPE Debug)

# output dirs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${SRC_DIR})

set(SRC_LIST
        ${SRC_DIR}/Polynomial.cpp
        ${SRC_DIR}/PolynomialRing.cpp
        ${SRC_DIR}/PolynomialField.cpp
        ${SRC_DIR}/Polynomial.hpp
        ${SRC_DIR}/PolynomialRing.hpp
        ${SRC_DIR}/PolynomialField.hpp
        ${SRC_DIR}/FieldMultiplicationCache.hpp
        )

set(LIB_NAME ${PROJECT_NAME}core)
#
add_library(${LIB_NAME} STATIC ${SRC_LIST})


option(ENABLE_TESTS "Build tests for project" ON)
if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(${TESTS_DIR})
endif()

# executable for quick testing in main.cpp
add_executable(main main.cpp)
target_link_libraries(main PRIVATE ${LIB_NAME})




