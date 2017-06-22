include_directories(../include/)

set(BBBLUEEEROS_SRCS BBBlueDevice.cpp DigOut.cpp DigIn.cpp AnalogOut.cpp Fqd.cpp)

add_library(bbblueeeros SHARED ${BBBLUEEEROS_SRCS})
# add_library(bbblueeeros_static ${BBBLUEEEROS_SRCS})

target_link_libraries(bbblueeeros roboticscape eeros)
# target_link_libraries(bbblueeeros_static roboticscape eeros)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/libbbblueeeros.so ${CMAKE_CURRENT_BINARY_DIR}/libbbblueeeros_static.a DESTINATION lib)
