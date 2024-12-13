# create executable from main .cc file
add_executable(doublecap doublecap.cc)

# include directories
target_include_directories(doublecap PUBLIC "${CMAKE_SOURCE_DIR}/include")

# link Geant and ROOT
target_link_libraries(doublecap PUBLIC "${Geant4_LIBRARIES}")
target_link_libraries(doublecap PUBLIC "${ROOT_LIBRARIES}")

# link source code
file(GLOB SOURCEFILES CONFIGURE_DEPENDS "DC*.cc")
target_sources(doublecap PRIVATE ${SOURCEFILES})

