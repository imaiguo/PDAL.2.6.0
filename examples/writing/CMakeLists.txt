cmake_minimum_required(VERSION 3.6)
project(WritingTutorial)

set(PDAL_DIR D:/devtools/PDAL.2.6.0/lib/cmake/PDAL)

find_package(PDAL 2.0.0 REQUIRED CONFIG)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tutorial tutorial.cpp)

target_link_libraries(tutorial PRIVATE ${PDAL_LIBRARIES})
target_include_directories(tutorial PRIVATE
    ${PDAL_INCLUDE_DIRS}
    ${PDAL_INCLUDE_DIRS}/pdal)


file(GLOB PDALDLLFILES  "D:/devtools/PDAL.2.6.0/bin/*.dll")

file(COPY ${PDALDLLFILES} DESTINATION ./)
