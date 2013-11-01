INCLUDE_DIRECTORIES("." "${CMAKE_BINARY_DIR}")
INCLUDE_DIRECTORIES("${EXPAT_INCLUDE_DIRS}")

# Ensure that the C source-files for the scanner and parser are up-to-date
# on a Unix system.
if (UNIX)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/scanner.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND flex -d -Put -o scanner.c scanner.l
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/scanner.l)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/parser.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND bison -t -p ut -o parser.c parser.y
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser.y)
    set_source_files_properties(parser.c
        PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/scanner.c)
endif()

add_library(libudunits2
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
target_link_libraries(libudunits2 ${EXPAT_LIBRARIES})

if (LD_CUNIT)
    add_executable(testUnits testUnits.c)
    target_include_directories(testUnits PUBLIC "${CUNIT_INCLUDE_DIR}")
    target_link_libraries (testUnits libudunits2)
    target_link_libraries (testUnits ${EXPAT_LIBRARIES})
    target_link_libraries (testUnits ${LD_CUNIT})
    target_link_libraries (testUnits ${LD_M})
    add_test(
        NAME testUnits
        COMMAND testUnits ${CMAKE_CURRENT_SOURCE_DIR}/udunits2.xml)
endif()

# The documentation is in multiple texinfo(5) format files
texi_doc(udunits2lib.texi ${CMAKE_SOURCE_DIR}/COPYRIGHT)
    
install(TARGETS libudunits2
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
install(FILES udunits2.h DESTINATION include)
install(FILES
        udunits2.xml
        udunits2-accepted.xml
        udunits2-base.xml
        udunits2-common.xml
        udunits2-derived.xml
        udunits2-prefixes.xml
    DESTINATION share/udunits)
