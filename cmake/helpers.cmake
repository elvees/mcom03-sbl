# SPDX-License-Identifier: MIT

macro(generate_ld_from_lds INPUT_FILE OUTPUT_FILE)
    #Set variable for depth file
    set(DEPFILE ${OUTPUT_FILE}.d)

    # Preprocesses file
    add_custom_command(OUTPUT ${OUTPUT_FILE} ${DEPFILE}
                       DEPENDS ${INPUT_FILE}
                       DEPFILE ${DEPFILE}
                       COMMAND ${CMAKE_C_COMPILER}
                       "-D$<JOIN:${COMPILE_DEFINITIONS},;-D>"
                       "-I$<JOIN:${INCLUDE_DIRECTORIES},;-I>"
                       "$<JOIN:${COMPILE_OPTIONS},;>"
                       -MD -MF ${DEPFILE}
                       -MT ${OUTPUT_FILE}
                       -MT ${DEPFILE}
                       -E ${INPUT_FILE} -P
                       -o ${OUTPUT_FILE}
                       COMMAND_EXPAND_LISTS
                       VERBATIM)
endmacro()

macro(generate_obj_from_bin INPUT_FILE OUTPUT_FILE)
    add_custom_command(OUTPUT ${OUTPUT_FILE}
                       DEPENDS ${INPUT_FILE}
                       COMMAND ${CMAKE_OBJCOPY} -I binary -O elf32-littlemips -B mips
                               --rename-section .data=.rodata ${INPUT_FILE} ${OUTPUT_FILE})
endmacro()

macro(generate_and_add_as_dependency TARGET INPUT_FILE OUTPUT_FILE)
    if(NOT TARGET ${TARGET})
        message(FATAL_ERROR "Target \"${TARGET}\" does not exist")
    endif()

    get_filename_component(IN_LAST_EXT "${INPUT_FILE}" LAST_EXT)
    get_filename_component(IN_FILE_NAME_WLE "${INPUT_FILE}" NAME_WLE)
    get_filename_component(IN_SECOND_LAST_EXT "${IN_FILE_NAME_WLE}" LAST_EXT)

    if("${IN_SECOND_LAST_EXT}${IN_LAST_EXT}" STREQUAL ".ld.S")
        # Extract properties from target
        get_target_property(TARGET_INCLUDE_DIRECTORIES ${TARGET} INCLUDE_DIRECTORIES)
        get_target_property(TARGET_COMPILE_DEFINITIONS ${TARGET} COMPILE_DEFINITIONS)
        get_target_property(TARGET_COMPILE_OPTIONS ${TARGET} COMPILE_OPTIONS)

        # Extract properties from directory
        get_property(INCLUDE_DIRECTORIES DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
        get_property(COMPILE_DEFINITIONS DIRECTORY PROPERTY COMPILE_DEFINITIONS)
        get_property(COMPILE_OPTIONS DIRECTORY PROPERTY COMPILE_OPTIONS)

        if (TARGET_INCLUDE_DIRECTORIES)
            set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${TARGET_INCLUDE_DIRECTORIES})
        endif()

        if (TARGET_COMPILE_DEFINITIONS)
            set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} ${TARGET_COMPILE_DEFINITIONS})
        endif()

        if (TARGET_COMPILE_OPTIONS)
            set(COMPILE_OPTIONS ${COMPILE_OPTIONS} ${TARGET_COMPILE_OPTIONS})
        endif()

        generate_ld_from_lds(${INPUT_FILE} ${OUTPUT_FILE})
    elseif("${IN_LAST_EXT}" STREQUAL ".bin" OR "${IN_LAST_EXT}" STREQUAL ".dtb")
        generate_obj_from_bin(${INPUT_FILE} ${OUTPUT_FILE})
    else()
        message(FATAL_ERROR "Unsupported file type \"${IN_LAST_EXT}\"")
    endif()

    get_target_property(TARGET_NAME ${TARGET} NAME)
    get_filename_component(OUT_FILE_NAME_WLE ${OUTPUT_FILE} NAME_WLE)

    set(NEW_TARGET "${TARGET_NAME}-LINK-DEPENDS-${OUT_FILE_NAME_WLE}")
    if(TARGET ${NEW_TARGET})
        message(FATAL_ERROR "File \"${OUTPUT_FILE}\" is already set as LINK_DEPENDS \
                property for target \"${TARGET}\" via \"generate_and_add_as_dependency\" macro")
    endif()

    # Sets dummy target to ensure that OUTPUT_FILE is generated before linking
    add_custom_target(${NEW_TARGET} DEPENDS ${OUTPUT_FILE})
    add_dependencies(${TARGET} ${NEW_TARGET})
    set_property(TARGET ${TARGET} APPEND PROPERTY LINK_DEPENDS "${OUTPUT_FILE}")
endmacro()
