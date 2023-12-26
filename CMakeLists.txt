cmake_minimum_required(VERSION 3.26)
project(martynkaprojekt)

set(CMAKE_CXX_STANDARD 14)

# Ustawianie ścieżki do pliku CSV
configure_file(${CMAKE_SOURCE_DIR}/intc_us_data.csv ${CMAKE_BINARY_DIR}/intc_us_data.csv COPYONLY)

add_executable(martynkaprojekt main.cpp)
