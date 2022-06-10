cmake_minimum_required(VERSION 3.21.1)
project(hydro_ion_life)

set(CMAKE_CXX_STANDARD 14)

add_executable(hydro_ion_life main.cpp classes_for_water/Atom.cpp classes_for_water/Atom.h classes_for_water/H_atom.cpp classes_for_water/H_atom.h classes_for_water/O_atom.cpp classes_for_water/O_atom.h parc_and_analis.h parc_and_analis.cpp)


find_package(zlib)

target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
