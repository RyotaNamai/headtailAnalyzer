cmake_minimum_required(VERSION 3.1.3)
set (CMAKE_CXX_STANDARD 11)
project(analog LANGUAGES CXX)

set(Targets eventSelector headtailAnalyzer slopeVisualizer)

# root
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})

include(${ROOT_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# pthread
if (NOT WIN32)
find_library(PTHREAD_LIBRARIES pthread)
endif()

FIND_PACKAGE(Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

add_executable(eventSelector "eventSelector.cpp")
add_executable(headtailAnalyzer "headtailAnalyzer.cpp")
add_executable(slopeVisualizer "slopeVisualizer.cpp")

target_link_libraries(eventSelector PRIVATE ROOT::RIO ROOT::Net ROOT::Tree)
target_link_libraries(headtailAnalyzer PRIVATE ROOT::RIO ROOT::Net ROOT::Tree ROOT::Hist ROOT::Gpad)
target_link_libraries(slopeVisualizer PRIVATE ROOT::RIO ROOT::Net ROOT::Tree ROOT::Hist ROOT::Gpad)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/..)
install(TARGETS ${Targets} DESTINATION bin)