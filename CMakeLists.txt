PROJECT (udunits C)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(udunits_VERSION_MAJOR 2)
SET(udunits_VERSION_MINOR 1)
SET(udunits_VERSION_PATCH 26)
SET(DEFAULT_UDUNITS2_XML_PATH "${CMAKE_INSTALL_PREFIX}/share/udunits/udunits2.xml")

INCLUDE(CheckFunctionExists)
INCLUDE(CTest)
# The following fails on Gilda:
#INCLUDE(GNUInstallDirs)
# Hence:
SET(CMAKE_INSTALL_INFODIR share/info)
SET(CMAKE_INSTALL_FULL_INFODIR share/info)

# Creates an info(1)-file from texinfo(5) input
find_program(MAKEINFO makeinfo)
function(make_info output input)
    add_custom_command(
        OUTPUT ${output}
        MAIN_DEPENDENCY ${input}
        DEPENDS ${ARGN}
        COMMAND ${MAKEINFO} -o ${output} -I ${CMAKE_SOURCE_DIR} ${input}
        VERBATIM
        COMMENT "Creating info(1) file ${output}")
endfunction(make_info)

# Installs an info(1)-file
find_program(INSTALL_INFO install-info)
IF (NOT INSTALL_INFO)
    MESSAGE("install-info(1) not found. Top-level info(1)-file will not be adjusted.")
ELSE()
    EXECUTE_PROCESS(COMMAND ${INSTALL_INFO} --version OUTPUT_QUIET
            RESULT_VARIABLE status)
    IF(NOT status EQUAL "0")
        MESSAGE("install-info(1) failure. Top-level info(1)-file will not be adjusted.")
        SET(INSTALL_INFO "NO")
    ELSE()
        EXECUTE_PROCESS(COMMAND ${INSTALL_INFO} --version COMMAND head -1
            COMMAND grep -q -i -v debian RESULT_VARIABLE status)
        IF(NOT status EQUAL "0")
            MESSAGE("Debian system. Top-level info(1)-file will not be adjusted.")
            SET(INSTALL_INFO "NO")
        ENDIF()
    ENDIF()
ENDIF()
FUNCTION(INSTALL_INFO input)
    INSTALL(FILES ${input} DESTINATION ${CMAKE_INSTALL_INFODIR})
    IF(INSTALL_INFO)
        SET(relOutput "${CMAKE_INSTALL_INFODIR}/${input}")
        add_custom_command(
            OUTPUT "${CMAKE_INSTALL_INFODIR}/dir"
            MAIN_DEPENDENCY "${relOutput}"
            COMMAND ${INSTALL_INFO} --info-dir="${CMAKE_INSTALL_FULL_INFODIR}"
                "${CMAKE_INSTALL_FULL_INFODIR}/${input}"
            VERBATIM
            COMMENT "Adjusting top-level info(1)-file according to ${relOutput}")
#       add_custom_command(
#           OUTPUT "${relOutput}"
#           MAIN_DEPENDENCY "${input}"
#           COMMAND ${INSTALL_INFO} --info-dir="${CMAKE_INSTALL_FULL_INFODIR}"
#               "${CMAKE_INSTALL_FULL_INFODIR}/${input}"
#           VERBATIM
#           COMMENT "Adjusting top-level info(1)-file according to ${relOutput}")
# The following doesn't work because "adjust_top_info" isn't an executable,...
#       add_custom_target(adjust_top_info 
#           COMMAND ${INSTALL_INFO} --info-dir="${CMAKE_INSTALL_FULL_INFODIR}"
#               "${CMAKE_INSTALL_FULL_INFODIR}/${input}"
#           VERBATIM
#           DEPENDS "${CMAKE_INSTALL_FULL_INFODIR}/${input}")
#       install(TARGETS adjust_top_info)
    ENDIF()
ENDFUNCTION()

# This package uses math functions
CHECK_FUNCTION_EXISTS(log10 HAVE_LOG10)
IF (NOT HAVE_LOG10)
    FIND_LIBRARY(LD_M NAMES math m)
    IF(NOT LD_M)
        MESSAGE(FATAL_ERROR "Unable to find the math library.")
    ENDIF()
ENDIF()

# The EXPAT (SAX XML parser) library is used to parse the units database
FIND_LIBRARY(LD_EXPAT expat)
IF(NOT LD_EXPAT)
    MESSAGE(FATAL_ERROR "Unable to find the EXPAT library.")
ENDIF()

# The CUnit library is used for unit-testing
FIND_LIBRARY(LD_CUNIT cunit)
IF(LD_CUNIT)
    MESSAGE(STATUS "Found CUnit library: ${LD_CUNIT}")
ELSE()
    MESSAGE("Unable to find the CUnit library. Unit-testing disabled.")
ENDIF()

CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/config.h.cmake"
    "${PROJECT_BINARY_DIR}/config.h")
# Necessary for the compiler to find "config.h":
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")

ENABLE_TESTING()

ADD_SUBDIRECTORY (lib)
ADD_SUBDIRECTORY (prog)

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Support for units of physical quantities")
SET(CPACK_PACKAGE_VENDOR "University Corporation for Atmospheric Research")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYRIGHT")
SET(CPACK_PACKAGE_VERSION_MAJOR ${udunits_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${udunits_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${udunits_VERSION_PATCH})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "udunits-${udunits_VERSION_MAJOR}.${udunits_VERSION_MINOR}")
#IF(WIN32 AND NOT UNIX)
#  # There is a bug in NSI that does not handle full unix paths properly. Make
#  # sure there is at least one set of four (4) backlasshes.
#  SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
#  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
#  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} My Famous Project")
#  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.my-project-home-page.org")
#  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.my-personal-home-page.com")
#  SET(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")
#  SET(CPACK_NSIS_MODIFY_PATH ON)
#ELSE(WIN32 AND NOT UNIX)
  SET(CPACK_STRIP_FILES "bin/udunits2")
  SET(CPACK_SOURCE_STRIP_FILES "")
#ENDIF(WIN32 AND NOT UNIX)
SET(CPACK_PACKAGE_EXECUTABLES "udunits2" "udunits2")
SET(CPACK_GENERATOR "Unix Makefiles")
INCLUDE(CPack)


# The documentation is in texinfo(5) format
find_program(MAKEINFO makeinfo)
make_info(udunits2.info ${CMAKE_CURRENT_SOURCE_DIR}/udunits2.texi
    ${CMAKE_CURRENT_SOURCE_DIR}/version.texi
    ${CMAKE_CURRENT_SOURCE_DIR}/COPYRIGHT)
add_custom_target(udunits2_info ALL DEPENDS udunits2.info)

# The documentation is in texinfo(5) format and must be composed
#set(results_tab ${CMAKE_CURRENT_SOURCE_DIR}/test/results.tab)
#add_custom_command(
#    OUTPUT success.texi
#    DEPENDS ${results_tab}
#    COMMAND sort -u -t : -k 2,2 -k 4,4 -k 5,5 ${results_tab} |
#	awk -F: ' 
#	    BEGIN { 
#		print \"@multitable {@code{Linux 2.6.18-1.2257.fc5smp}} {@code{/opt/csw/gcc4/bin/gcc}} {@code{--disable-shared}}\"; 
#		print \"@headitem O/S @tab Compiler @tab @code{configure} Option\";
#	    } 
#	    $$6 == 1 { 
#		printf \"@item @code{%s} @tab @code{%s} @tab @code{%s}\\n\", 
#		    $$2, $$4, $$5; 
#	    } 
#	    END { 
#		print \"@end multitable\"; 
#	    }' >success.texi)
#find_program(MAKEINFO makeinfo)
#set(texinfo_in ${CMAKE_CURRENT_SOURCE_DIR}/udunits2.texi)
#    ${CMAKE_CURRENT_BINARY_DIR}/success.texi
#    ${CMAKE_CURRENT_BINARY_DIR}/failure.texi
#    ${CMAKE_CURRENT_BINARY_DIR}/make.texi)
#set(texinfo_in ${CMAKE_CURRENT_SOURCE_DIR}/udunits2.texi)
#add_custom_command(
#    OUTPUT udunits2.info
#    DEPENDS ${texinfo_in}
#    COMMAND ${MAKEINFO} -o udunits2.info ${texinfo_in}
#    COMMENT "Creating Info file udunits2.info"
#    VERBATIM)
#add_custom_target(texinfo ALL DEPENDS udunits2.info)
