include_directories ("${PROJECT_SOURCE_DIR}/lib")

link_directories(${PROJECT_BINARY_DIR}/lib)

add_executable(udunits2 udunits2.c)

target_link_libraries(udunits2 libudunits2)
target_link_libraries(udunits2 ${EXPAT_LIBRARIES})
target_link_libraries(udunits2 ${LD_M})
set_target_properties(udunits2 PROPERTIES
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
    INSTALL_RPATH_USE_LINK_PATH "TRUE")

# The documentation is in multiple texinfo(5) format files
texi_doc(udunits2prog.texi ${CMAKE_SOURCE_DIR}/COPYRIGHT)

install(TARGETS udunits2 DESTINATION bin)
