INCLUDE_DIRECTORIES(".")

add_library(libudunits2 SHARED
    converter.c 
    error.c
    formatter.c
    idToUnitMap.c 
    parser.c
    prefix.c
    status.c
    systemMap.c 
    unitAndId.c 
    unitcore.c 
    unitToIdMap.c 
    ut_free_system.c
    xml.c)
set_target_properties(libudunits2 PROPERTIES LIBRARY_OUTPUT_NAME udunits2)
set_target_properties(libudunits2 PROPERTIES ARCHIVE_OUTPUT_NAME udunits2)
set_target_properties(libudunits2 PROPERTIES RUNTIME_OUTPUT_NAME udunits2)

IF (LD_CUNIT)
    ADD_EXECUTABLE(testUnits testUnits.c)
    target_link_libraries (testUnits libudunits2)
    target_link_libraries (testUnits ${LD_EXPAT})
    target_link_libraries (testUnits ${LD_CUNIT})
    target_link_libraries (testUnits ${LD_M})
    ADD_TEST(
        NAME testUnits
        COMMAND testUnits ${CMAKE_CURRENT_SOURCE_DIR}/udunits2.xml)
ENDIF()

# The documentation is in multiple texinfo(5) format files
make_info(udunits2lib.info ${CMAKE_CURRENT_SOURCE_DIR}/udunits2lib.texi
    ${CMAKE_SOURCE_DIR}/COPYRIGHT)
ADD_CUSTOM_TARGET(udunits2lib_info ALL DEPENDS udunits2lib.info)

install(TARGETS libudunits2 DESTINATION lib)
install(FILES udunits2.h DESTINATION include)
install(FILES
        udunits2.xml
        udunits2-accepted.xml
        udunits2-base.xml
        udunits2-common.xml
        udunits2-derived.xml
        udunits2-prefixes.xml
    DESTINATION share/udunits)
INSTALL_INFO(udunits2lib.info)