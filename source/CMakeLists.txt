cmake_minimum_required(VERSION 3.1.3)
set (CMAKE_CXX_STANDARD 11)
project(analog LANGUAGES CXX)

set(Targets eventSelector headtailAnalyzer)

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

# boost
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS *boost libraries here*) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(progname file1.cxx file2.cxx) 
    target_link_libraries(progname ${Boost_LIBRARIES})
endif()

add_executable(eventSelector "eventSelector.cpp")
add_executable(headtailAnalyzer "headtailAnalyzer.cpp")

target_link_libraries(eventSelector PRIVATE ROOT::RIO ROOT::Net ROOT::Tree)
target_link_libraries(headtailAnalyzer PRIVATE ROOT::RIO ROOT::Net ROOT::Tree ROOT::Hist ROOT::Gpad)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/..)
install(TARGETS ${Targets} DESTINATION bin)