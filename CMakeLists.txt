set(GEN_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
set(SASPPU_GEN_INCLUDE_DIR "${GEN_INCLUDE_DIR}/sasppu")

if (NOT EXISTS ${SASPPU_GEN_INCLUDE_DIR})
    file(MAKE_DIRECTORY ${SASPPU_GEN_INCLUDE_DIR})
endif()

set(SASPPU_GEN "${SASPPU_GEN_INCLUDE_DIR}/gen.h")

idf_component_register(
    SRCS 
        src/help.c
        src/sasppu.c
    INCLUDE_DIRS
        include/public
    PRIV_INCLUDE_DIRS
        include/private
        ${GEN_INCLUDE_DIR}
)

# Generate sasppu/gen.h
add_custom_command(OUTPUT ${SASPPU_GEN}
                   COMMAND ${python} ${CMAKE_CURRENT_SOURCE_DIR}/src/generated.py ${SASPPU_GEN}
                   MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/src/generated.py
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src
                   COMMENT "Generating sasppu/gen.h"
                   )
        
add_custom_target(generate_sasppu_gen ALL DEPENDS ${SASPPU_GEN})
idf_component_get_property(sasppu_lib sasppu COMPONENT_LIB)
add_dependencies(${sasppu_lib} generate_sasppu_gen)

target_compile_options(${sasppu_lib} PRIVATE "-O3")

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

if( supported )
    message(STATUS "IPO / LTO enabled")
    set_property(TARGET ${sasppu_lib} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    target_link_options(${sasppu_lib} PUBLIC "-flto")
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()

set_source_files_properties(${SASPPU_GEN} PROPERTIES HEADER_FILE_ONLY ON)

component_compile_options("-Wa,-I${CMAKE_CURRENT_SOURCE_DIR}/include/private")
