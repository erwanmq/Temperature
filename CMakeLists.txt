cmake_minimum_required(VERSION 3.20)

project(Temperature C)

#===============================================================================
# Platform selection
#===============================================================================

set(TARGET_PLATFORM "linux" CACHE STRING "Target platform")

set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS
    linux
    at89c51rb2
)

#===============================================================================
# Common settings
#===============================================================================

set(INC_DIR ${CMAKE_SOURCE_DIR}/inc)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(${INC_DIR})

set(COMMON_SOURCES
    ${SRC_DIR}/main.c
    ${SRC_DIR}/application/FSM.c
)

#===============================================================================
# Platform specific sources
#===============================================================================

if(TARGET_PLATFORM STREQUAL "linux")

    message(STATUS "Building for Linux")

    set(PLATFORM_SOURCES
        ${SRC_DIR}/hal/linux/hal_display.c
        ${SRC_DIR}/hal/linux/hal_timer.c
        ${SRC_DIR}/hal/linux/hal_spi.c
        ${SRC_DIR}/hal/linux/hal_adc.c
    )

elseif(TARGET_PLATFORM STREQUAL "at89c51rb2")

    message(STATUS "Building for AT89C51RB2")

    set(PLATFORM_SOURCES
        ${SRC_DIR}/hal/at89c51rb2/hal_display.c
        ${SRC_DIR}/hal/at89c51rb2/hal_timer.c
        ${SRC_DIR}/hal/at89c51rb2/hal_spi.c
        ${SRC_DIR}/hal/at89c51rb2/hal_adc.c

        ${SRC_DIR}/drivers/at89c51rb2/timer/timer2.c
        ${SRC_DIR}/drivers/at89c51rb2/spi/spi.c
        ${SRC_DIR}/drivers/at89c51rb2/sleep/sleep.c
        ${SRC_DIR}/drivers/at89c51rb2/external_interrupts/external_interrupts.c
        ${SRC_DIR}/drivers/at89c51rb2/segments_display/segments_display.c
        ${SRC_DIR}/drivers/at89c51rb2/mcp3008/mcp3008.c
        ${SRC_DIR}/utils/stack.c
    )

else()

    message(FATAL_ERROR "Unknown platform: ${TARGET_PLATFORM}")

endif()

#===============================================================================
# Target
#===============================================================================

add_executable(${PROJECT_NAME}
    ${COMMON_SOURCES}
    ${PLATFORM_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${INC_DIR}
)

#===============================================================================
# SDCC specific
#===============================================================================

if(TARGET_PLATFORM STREQUAL "at89c51rb2")

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD

        COMMAND sdcc-packihx
                ${PROJECT_NAME}.ihx
                >
                ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex

        COMMENT "Generating Intel HEX"
    )

endif()
