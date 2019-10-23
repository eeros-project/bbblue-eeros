cmake_minimum_required(VERSION 3.5.1)

project(bbblue-eeros)

set(CMAKE_CXX_STANDARD 14)


set(RECOMMENDED_EEROS_VERSION 1.1.0.0)


if(CMAKE_BUILD_TYPE MATCHES "Debug|DEBUG")
	set(LOGGER ON)
endif()


include(cmake/versionCheckHelpers.cmake)


setLibraryVersion()


# Find EEROS
setVersionToFind("EEROS" "REQUIRED_EEROS_VERSION" ${RECOMMENDED_EEROS_VERSION} ${REQUIRED_EEROS_VERSION})
find_package(EEROS ${VERSION_TO_FIND} EXACT)
checkIfModuleFound("EEROS" "REQUIRED_EEROS_VERSION")


include_directories(${ADDITIONAL_INCLUDE_DIRS})
link_directories(${ADDITIONAL_LINK_DIRS})

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


createPackageFiles(bbblue-eerosConfig.cmake.in include/ lib/)


INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/BBBlueDevice.hpp 
	      ${CMAKE_CURRENT_SOURCE_DIR}/include/DigOut.hpp 
	      ${CMAKE_CURRENT_SOURCE_DIR}/include/DigIn.hpp 
	      ${CMAKE_CURRENT_SOURCE_DIR}/include/AnalogOut.hpp 
	      ${CMAKE_CURRENT_SOURCE_DIR}/include/AnalogIn.hpp 
	      ${CMAKE_CURRENT_SOURCE_DIR}/include/Fqd.hpp  DESTINATION include/bbblueeeros)

