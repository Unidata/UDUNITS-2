include_directories ("${PROJECT_SOURCE_DIR}/lib")

link_directories (${PROJECT_BINARY_DIR}/lib)

add_executable(udunits2 udunits2.c)

target_link_libraries (udunits2 libudunits2)
target_link_libraries (udunits2 ${LD_EXPAT})
target_link_libraries (udunits2 ${LD_M})

# The documentation is in multiple texinfo(5) format files
make_info(udunits2prog.info ${CMAKE_CURRENT_SOURCE_DIR}/udunits2prog.texi
    ${CMAKE_SOURCE_DIR}/COPYRIGHT)
add_custom_target(udunits2prog ALL DEPENDS udunits2prog.info)

install(TARGETS udunits2 DESTINATION bin)