CMAKE_MINIMUM_REQUIRED(VERSION 3.19)

PROJECT(udunits VERSION 2.2.28
        DESCRIPTION "C library and utility for units of physical quantities")
set(CMAKE_PROJECT_HOMEPAGE_URL https://www.unidata.ucar.edu/software/udunits)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/.." CACHE PATH
        "Installation root directory" FORCE)
endif()

# Ensure a path in the format common to both Unix and MinGW.
FUNCTION(to_common_path input result)
    FILE(TO_NATIVE_PATH ${input} tmp)
    STRING(REGEX REPLACE "^(.):" "/\\1" tmp ${tmp})
    SET(${result} ${tmp} PARENT_SCOPE)
ENDFUNCTION(to_common_path)

# Build a shared library by default
INCLUDE(GenerateExportHeader)
option(BUILD_SHARED_LIBS "Build a shared library" ON)
if(BUILD_SHARED_LIBS)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CMAKE_BUILD_RPATH "$ORIGIN/../lib")
        set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(CMAKE_BUILD_RPATH "@executable_path/../../lib")
        set(CMAKE_INSTALL_RPATH "@executable_path/../../lib")
    ELSE()
        MESSAGE(WARNING "Relocatable binary distribution is not supported for "
                "O/S \"${CMAKE_SYSTEM_NAME}\"")
    ENDIF()
endif()

# A little bit of Windows/Visual Studio housekeeping.
IF(MSVC)
    # Set a property used with Visual Studio
    SET(GLOBAL PROPERTY USE_FOLDERS ON)

    # Suppress CRT warnings.
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)

    OPTION(ENABLE_DLL "Build a Windows DLL." ${BUILD_SHARED_LIBS})
    MESSAGE(STATUS "Building Windows DLL: ${ENABLE_DLL}")
    IF(ENABLE_DLL)
       SET(BUILD_DLL ON CACHE BOOL "")
       ADD_DEFINITIONS(-DDLL_UDUNITS2)
       ADD_DEFINITIONS(-DDLL_EXPORT)
    ENDIF()
ENDIF()

###
# Fix to make sure we don't try to include unistd.h
# when using Visual Studio.
###
INCLUDE(${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD_H)
IF(NOT HAVE_UNISTD_H)
  SET(YY_NO_UNISTD_H TRUE)
ENDIF()

# Ensures a path in the native format.
#FUNCTION(to_native_path input result)
#    FILE(TO_NATIVE_PATH ${input} tmp)
#    IF (WIN32 AND NOT UNIX)
#        STRING(REGEX REPLACE "/" "\\\\" tmp "${tmp}")
#    ENDIF()
#    SET(${result} "${tmp}" PARENT_SCOPE)
#ENDFUNCTION(to_native_path)

#FILE(TO_NATIVE_PATH "${CMAKE_INSTALL_PREFIX}/share/udunits/udunits2.xml"
#    DEFAULT_UDUNITS2_XML_PATH)
FILE(TO_NATIVE_PATH "${CMAKE_INSTALL_PREFIX}/share/udunits/udunits2.xml" tmp)
#MESSAGE("tmp = \"${tmp}\"")

STRING(REGEX REPLACE "\\\\ " " " tmp "${tmp}")
#MESSAGE("tmp = \"${tmp}\"")
#STRING(REGEX REPLACE "\\\\" "\\\\\\\\" DEFAULT_UDUNITS2_XML_PATH "${tmp}")

# Replace '\' with '\\' on Windows.
IF(MSVC)
    STRING(REPLACE "\\" "\\\\" tmp "${tmp}")
ENDIF(MSVC)

SET(DEFAULT_UDUNITS2_XML_PATH "${tmp}")
MESSAGE("DEFAULT_UDUNITS2_XML_PATH = \"${DEFAULT_UDUNITS2_XML_PATH}\"")

INCLUDE(CheckFunctionExists)
INCLUDE(CTest)
enable_testing()

# The following fails on Gilda:
#INCLUDE(GNUInstallDirs)
# Hence:
SET(CMAKE_INSTALL_INFODIR share/info)
SET(CMAKE_INSTALL_DOCDIR share/doc/udunits)

to_common_path(${CMAKE_SOURCE_DIR} common_source_dir)
to_common_path(${CMAKE_INSTALL_INFODIR} common_install_infodir)

# Determines if the top-level info(1)-file, "dir", should be modified.
find_program(INSTALL_INFO install-info)
IF (NOT INSTALL_INFO)
    MESSAGE("install-info(1) not found. Top-level info(1)-file will not be modified.")
ELSE()
    EXECUTE_PROCESS(COMMAND ${INSTALL_INFO} --version OUTPUT_QUIET
            RESULT_VARIABLE status)
    IF(NOT status EQUAL "0")
        MESSAGE("install-info(1) failure. Top-level info(1)-file will not be modified.")
        SET(INSTALL_INFO "NO")
    ELSE()
        EXECUTE_PROCESS(COMMAND ${INSTALL_INFO} --version COMMAND head -1
            COMMAND grep -q -i -v debian RESULT_VARIABLE status)
        IF(NOT status EQUAL "0")
            MESSAGE("Debian system. Top-level info(1)-file will not be modified.")
            SET(INSTALL_INFO "NO")
        ENDIF()
    ENDIF()
ENDIF()

# Declares documentation in texinfo(5) format. If "input" is relative, then it's
# resolved using the current source-directory. A custom target is derived from
# the filename and added to the default build target. The resulting
# documentation files are added to the "install" target.
find_program(MAKEINFO makeinfo)
IF (NOT MAKEINFO)
    MESSAGE(WARNING "makeinfo(1) not found. Existing documentation will be used.")
ENDIF()
FUNCTION(texi_doc input)
    string(REGEX REPLACE ".*/" "" basename ${input})
    string(REGEX REPLACE "\\..*" "" file ${basename})
    to_common_path(${input} common_input)
    IF (NOT MAKEINFO)
        MESSAGE("Texinfo-file \"${common_input}\" will not be processed.")
    ELSE()
        add_custom_command(
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${file}.info"
                "${CMAKE_CURRENT_SOURCE_DIR}/${file}.html"
            MAIN_DEPENDENCY ${input}
            DEPENDS ${ARGN}
            COMMAND ${MAKEINFO} -o ${file}.info -I ${common_source_dir}
                ${common_input}
            COMMAND ${MAKEINFO} -o ${file}.html -I ${common_source_dir}
                --html --no-split ${common_input}
            VERBATIM
            COMMENT "Creating documentation from ${common_input}")
    ENDIF()
    add_custom_target(${file}_doc ALL
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${file}.info
            ${CMAKE_CURRENT_SOURCE_DIR}/${file}.html)
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${file}.info" DESTINATION
            ${CMAKE_INSTALL_INFODIR})
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${file}.html" DESTINATION
            ${CMAKE_INSTALL_DOCDIR})
    if(INSTALL_INFO)
        SET(outpath "${common_install_infodir}/${file}.info")
        add_custom_command(
            OUTPUT "${CMAKE_INSTALL_INFODIR}/dir"
            MAIN_DEPENDENCY "${outpath}"
            COMMAND ${INSTALL_INFO} --info-dir="${common_install_infodir}"
                "${outpath}"
            VERBATIM
            COMMENT "Adjusting top-level info(1)-file according to ${outpath}")
    endif()
ENDFUNCTION(texi_doc)

# This package uses math functions
CHECK_FUNCTION_EXISTS(log10 HAVE_LOG10)
IF (NOT HAVE_LOG10)
    FIND_LIBRARY(MATH_LIBRARY NAMES m math)
    IF(NOT MATH_LIBRARY)
        MESSAGE(FATAL_ERROR "Unable to find a C math library.")
    ENDIF()
ENDIF()

# The EXPAT library, which implements a SAX XML parser, is used to parse the
# units database
INCLUDE(FindEXPAT)
IF(NOT EXPAT_FOUND)
    MESSAGE(FATAL_ERROR "Unable to find an EXPAT library.")
    IF(NOT EXPAT_INCLUDE_DIRS)
        MESSAGE(FATAL_ERROR "Unable to find an EXPAT header-file.")
    ENDIF()
ENDIF()

# The CUnit library is used for unit-testing
FIND_LIBRARY(CUNIT_LIBRARY cunit)
IF(CUNIT_LIBRARY)
    FIND_PATH(CUNIT_INCLUDE_DIR "CUnit/CUnit.h")
    IF (NOT CUNIT_INCLUDE_DIR)
        MESSAGE(STATUS "Unable to find CUnit header-file. Unit-testing not enabled.")
    ELSE()
        MESSAGE(STATUS "Found CUnit. Unit-testing enabled.")
        ENABLE_TESTING()
    ENDIF()
ELSE()
    MESSAGE(STATUS "Unable to find CUnit library. Unit-testing not enabled.")
ENDIF()

CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/config.h.cmake"
    "${PROJECT_BINARY_DIR}/config.h")
# Necessary for the compiler to find "config.h":
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")

ADD_SUBDIRECTORY (lib)

# Import symbols instead of export them.
IF(BUILD_DLL)
    REMOVE_DEFINITIONS(-DDLL_EXPORT)
ENDIF()

ADD_SUBDIRECTORY (prog)

# The documentation is in texinfo(5) format
texi_doc(udunits2.texi version.texi COPYRIGHT)

# Install CHANGE_LOG and COPYRIGHT
install(FILES "${CMAKE_SOURCE_DIR}/CHANGE_LOG" DESTINATION
            ${CMAKE_INSTALL_DOCDIR})
install(FILES "${CMAKE_SOURCE_DIR}/COPYRIGHT" DESTINATION
            ${CMAKE_INSTALL_DOCDIR})

# Test the installation
SET(util "${CMAKE_INSTALL_PREFIX}/bin/udunits2")
IF(WIN32)
    STRING(REGEX REPLACE "/" "\\\\" util "${util}")
ENDIF()
#MESSAGE("util = \"${util}\"")
ADD_CUSTOM_TARGET(install_test "${util}" -A -H km -W m
    COMMENT "Checking installation...")

# Create an installer
INCLUDE(InstallRequiredSystemLibraries)
# Apparently, CMAKE_SYSTEM_PROCESSOR can't be overridden via the "-D..." option.
IF (WIN32)
    SET(CPACK_GENERATOR "NSIS")
    GET_FILENAME_COMPONENT(CPACK_NSIS_INSTALL_ROOT "${CMAKE_INSTALL_PREFIX}" PATH)
    GET_FILENAME_COMPONENT(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_INSTALL_PREFIX}" NAME)
    STRING(REGEX REPLACE "/" "\\\\" CPACK_PACKAGE_INSTALL_DIRECTORY
        "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} UDUNITS-2")
    SET(CPACK_NSIS_HELP_LINK "http://www.unidata.ucar.edu/software/udunits")
    SET(CPACK_NSIS_MODIFY_PATH ON)
    SET(CPACK_NSIS_URL_INFO_ABOUT "${CPACK_NSIS_HELP_LINK}")
#   # There is a bug in NSI that does not handle full unix paths properly. Make
#   # sure there is at least one set of four (4) backslashes.
#   SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
#   SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
ENDIF()

SET(CPACK_DEBIAN_PACKAGE_DEPENDS "expat, libexpat-dev")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${CMAKE_HOST_SYSTEM_PROCESSOR}")
SET(CPACK_RPM_PACKAGE_REQUIRES "expat, expat-devel")
SET(CPACK_RPM_PACKAGE_PROVIDES "udunits2(1), libudunits2, udunits2.h, converter.h")

SET(CPACK_PACKAGE_FILE_NAME
        "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${CMAKE_HOST_SYSTEM_PROCESSOR}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME 
        "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}")
SET(CPACK_PACKAGE_CONTACT "support-udunits@unidata.ucar.edu")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
SET(CPACK_PACKAGE_EXECUTABLES "udunits2")
SET(CPACK_PACKAGE_VENDOR "University Corporation for Atmospheric Research")
SET(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYRIGHT")
SET(CPACK_PACKAGE_RELOCATABLE "ON")
SET(CPACK_GENERATOR "RPM;DEB")

SET(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
        /\\.
        ~$
        \\.t2p/
        \\.o$
        \\.lo$
        \\.la$
        \\.libs/
        /test/
        /autom4te\\.cache/
        /build/
        /conda\\.recipe/
        /consistent$
        /udunits2$
        \\.log$
        /appveyor\\.yml$
        /BACKLOG$
        /build\\.gradle$
        /check.*$
        /commit$
        /config\\.status$
        /dev-makefile$
        /Makefile$
        /publish$
        /push$
        /scan$
        /stamp-h1$
        /t.t$
        \\.gz$
        \\.rpm$
        /yum-upload$
)

SET(CPACK_VERBATIM_VARIABLES YES)
# The following causes cpack(1) to fail
INCLUDE(CPack)
#include(UseRPMTools)
#    if(RPMTools_ADD_RPM_TARGETS("udunits"))
#endif()
