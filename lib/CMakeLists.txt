include_directories(../include/)

set(BBBLUEEEROS_SRCS BBBlueDevice.cpp DigOut.cpp DigIn.cpp AnalogOut.cpp AnalogIn.cpp Fqd.cpp)

add_library(bbblueeeros SHARED ${BBBLUEEEROS_SRCS})
# add_library(bbblueeeros_static ${BBBLUEEEROS_SRCS})

target_link_libraries(bbblueeeros robotcontrol eeros)
# target_link_libraries(bbblueeeros_static robotcontrol eeros)


set_target_properties(bbblueeeros PROPERTIES VERSION ${LIBRARY_VERSION})

INSTALL(TARGETS bbblueeeros LIBRARY DESTINATION lib)
