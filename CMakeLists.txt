cmake_minimum_required(VERSION 3.5)

project(py_embedded_sample)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/pybind11/cmake")

include(pybind11Tools)


set(CMAKE_CXX_STANDARD 14)

if(WIN32)
    ADD_DEFINITIONS(-DUNICODE)
    ADD_DEFINITIONS(-D_UNICODE)
endif(WIN32)

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/pybind11/include)

# sample01
add_executable(sample01 sample01.cpp)
target_link_libraries(sample01 ${PYTHON_LIBRARIES})

add_custom_command(TARGET sample01 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/sample01.py
    ${CMAKE_CURRENT_BINARY_DIR}/sample01.py
)

# sample02
pybind11_add_module(sample02 sample02.cpp)
add_custom_command(TARGET sample02 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/sample02.py
    ${CMAKE_CURRENT_BINARY_DIR}/sample02.py
)