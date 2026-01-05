cmake_minimum_required(VERSION 3.10)
project(KittiMOT)

set(CMAKE_CXX_STANDARD 17)

# Find OpenCV
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(include)

# Add all source files
file(GLOB SOURCES "src/*.cpp")

add_executable(kitti_mot ${SOURCES})
target_link_libraries(kitti_mot ${OpenCV_LIBS})
