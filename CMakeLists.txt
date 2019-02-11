cmake_minimum_required(VERSION 3.5.1)
project(bbblueeeros)

set(CMAKE_CXX_STANDARD 14)

set(RECOMMENDED_EEROS_VERSION 1.0.0.0)

include(cmake/MunkeiVersionFromGit.cmake)
version_from_git() # Fetch the library version information from git

include_directories(${ADDITIONAL_INCLUDE_DIRS})
link_directories(${ADDITIONAL_LINK_DIRS})


if(NOT DEFINED REQUIRED_EEROS_VERSION OR
   NOT REQUIRED_EEROS_VERSION MATCHES
       "^(0|[1-9][0-9]*)[.](0|[1-9][0-9]*)[.](0|[1-9][0-9]*)[.](0|[1-9][0-9]*)$")

  set(REQUIRED_EEROS_VERSION ${RECOMMENDED_EEROS_VERSION})
  message("Using recommended EEROS version, which is: v${REQUIRED_EEROS_VERSION}")

else()
  if(NOT ${REQUIRED_EEROS_VERSION} EQUAL ${RECOMMENDED_EEROS_VERSION})
     message("You are not using the recommended EEROS version, "
             "which would be: v${RECOMMENDED_EEROS_VERSION}")
  endif(NOT ${REQUIRED_EEROS_VERSION} EQUAL ${RECOMMENDED_EEROS_VERSION})
endif()


find_package(EEROS ${REQUIRED_EEROS_VERSION} EXACT REQUIRED)
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
	      ${CMAKE_CURRENT_SOURCE_DIR}/include/Fqd.hpp  DESTINATION include/bbblueeeros)
