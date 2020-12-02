include_directories ("${PROJECT_SOURCE_DIR}/lib")

IF(BUILD_SHARED_LIBS AND MSVC)
 REMOVE_DEFINITIONS(-DDLL_EXPORT)
 REMOVE_DEFINITIONS(-DDLL_UDUNITS2)
ENDIF()


link_directories(${PROJECT_BINARY_DIR}/lib)

set(udunits2_SRC udunits2.c)

# We need to implement our own getopt if using Visual Studio.
IF(MSVC)
   set(udunits2_SRC ${udunits2_SRC} XGetOpt.c XGetOpt.h)
ENDIF()


add_executable(udunits2 ${udunits2_SRC})

target_link_libraries(udunits2 libudunits2)
IF(MSVC)
    SET_TARGET_PROPERTIES(udunits2 PROPERTIES LINK_FLAGS_DEBUG
    " /NODEFAULTLIB:MSVCRT"
    )
ENDIF()

# The documentation is in multiple texinfo(5) format files
texi_doc(udunits2prog.texi ${CMAKE_SOURCE_DIR}/COPYRIGHT)

install(TARGETS udunits2 DESTINATION bin)
