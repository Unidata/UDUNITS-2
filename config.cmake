find_program(MAKEINFO makeinfo)

# Creates an info(1)-file from texinfo(5) input
function(make_info output input)
    add_custom_command(
        OUTPUT ${output}
        MAIN_DEPENDENCY ${input}
        DEPENDS ${ARGN}
        COMMAND ${MAKEINFO} -o ${output} -I ${CMAKE_SOURCE_DIR} ${input}
        VERBATIM
        COMMENT "Creating info(1) file ${output}")
endfunction(make_info)