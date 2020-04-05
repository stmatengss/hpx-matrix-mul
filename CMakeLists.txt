cmake_minimum_required(VERSION 3.3.2)
project(hpx_matrix_multiple CXX)

list(APPEND CMAKE_PREFIX_PATH "$HOME/hpx")

find_package(HPX REQUIRED)

add_hpx_executable(${CMAKE_PROJECT_NAME}
	SOURCES ${CMAKE_PROJECT_NAME}.cpp
        COMPONENT_DEPENDENCIES iostreams
        )
