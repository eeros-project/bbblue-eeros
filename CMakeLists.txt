cmake_minimum_required(VERSION 3.5.1)
project(bbblueeeros)

set(CMAKE_CXX_STANDARD 14)

## fetch the version information from git ; Source: https://github.com/Munkei/munkei-cmake
## doc: https://github.com/Munkei/munkei-cmake/blob/master/doc/MunkeiVersionFromGit.md
include(cmake/MunkeiVersionFromGit.cmake)
version_from_git()

include_directories(${ADDITIONAL_INCLUDE_DIRS})
link_directories(${ADDITIONAL_LINK_DIRS})

find_package(EEROS ${REQUIRED_EEROS_VERSION} REQUIRED)
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
