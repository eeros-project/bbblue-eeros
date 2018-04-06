cmake_minimum_required(VERSION 2.6)
project(bbblueeeros)

## Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${ADDITIONAL_INCLUDE_DIRS})
link_directories(${ADDITIONAL_LINK_DIRS})

find_package(EEROS REQUIRED)
include_directories(${EEROS_INCLUDE_DIR})
link_directories(${EEROS_LIB_DIR})

find_path(BEAGLE_BOARD_BLUE_INCLUDE_DIR NAMES rc_usefulincludes.h roboticscape.h)
if(BEAGLE_BOARD_BLUE_INCLUDE_DIR)
  include_directories(${BEAGLE_BOARD_BLUE_INCLUDE_DIR})
endif()
find_library(BEAGLE_BOARD_BLUE_LIBRARY roboticscape)
if(BEAGLE_BOARD_BLUE_LIBRARY)
  message(STATUS "-> libroboticscape available")
  set(BEAGLE_BOARD_BLUE_LIBRARY_DIR ${BEAGLE_BOARD_BLUE_LIBRARY_DIR} roboticscape)
endif()

add_subdirectory(lib)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/BBBlueDevice.hpp 
	      ${CMAKE_CURRENT_SOURCE_DIR}/include/DigOut.hpp 
	      ${CMAKE_CURRENT_SOURCE_DIR}/include/DigIn.hpp 
	      ${CMAKE_CURRENT_SOURCE_DIR}/include/AnalogOut.hpp 
	      ${CMAKE_CURRENT_SOURCE_DIR}/include/AnalogIn.hpp
	      ${CMAKE_CURRENT_SOURCE_DIR}/include/Fqd.hpp  DESTINATION include/bbblueeeros)