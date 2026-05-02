cmake_minimum_required(VERSION 3.24)
set(CMAKE_C_STANDARD 11)

project(CorgoEngine C ASM)

# FetchContent consumers call into this file as a subproject.
# Keep standalone defaults only when configuring the engine repo directly.
set(CE_ENGINE_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "Corgo Engine root directory")
set(CE_ENGINE_IS_TOP_LEVEL OFF)
if (CMAKE_SOURCE_DIR STREQUAL CE_ENGINE_ROOT_DIR)
	set(CE_ENGINE_IS_TOP_LEVEL ON)
endif()

if (CE_ENGINE_IS_TOP_LEVEL)
	# Game Name — controls source directory (src/<name>/), artifact name (<name>.pdx),
	# and the CE_ENGINE_GAME_DIR compiler define. Override via CMake cache (e.g. in a preset).
	set(CE_GAME_NAME "game" CACHE STRING "Game name: drives src/<name>/ source glob and <name>.pdx artifact")
endif()

# Optional scene symbol used to define CE_ENGINE_SET_START_SCENE at compile time.
# Example: -DCE_ENGINE_START_SCENE=SpriteDemo
set(CE_ENGINE_START_SCENE "" CACHE STRING "Optional start scene symbol for CE_ENGINE_SET_START_SCENE (leave empty to skip define)")

include(CMakeParseArguments)
include(FetchContent)

set(ENVSDK $ENV{PLAYDATE_SDK_PATH})

if (NOT ${ENVSDK} STREQUAL "")
	# Convert path from Windows
	file(TO_CMAKE_PATH ${ENVSDK} SDK)
else()
	execute_process(
			COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
			COMMAND head -n 1
			COMMAND cut -c9-
			OUTPUT_VARIABLE SDK
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endif()

if (NOT EXISTS ${SDK})
	message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
	return()
endif()

set(CE_ENGINE_PLAYDATE_SDK "${SDK}" CACHE INTERNAL "Playdate SDK path used by Corgo Engine")

set(BUILD_SHARED_LIBS OFF)

# Override printf cache variables before FetchContent processes the subproject.
# These must use CACHE ... FORCE and be set before FetchContent_MakeAvailable.
set(SUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS OFF CACHE BOOL "Disable MSVC-style integer specifiers in printf" FORCE)
set(USE_DOUBLE_INTERNALLY OFF CACHE BOOL "Disable double support" FORCE)
if (TOOLCHAIN STREQUAL "armgcc")
	set(ALIAS_STANDARD_FUNCTION_NAMES SOFT CACHE STRING "Alias standard printf names (SOFT) for ARM device build" FORCE)
else()
	set(ALIAS_STANDARD_FUNCTION_NAMES NONE CACHE STRING "Alias standard printf names" FORCE)
endif()

FetchContent_Declare(printf_library
	GIT_REPOSITORY https://github.com/eyalroz/printf.git
	GIT_TAG v6.4.0-b1
	SOURCE_DIR ${CE_ENGINE_ROOT_DIR}/3rdParty/printf
)
FetchContent_MakeAvailable(printf_library)
set(CE_ENGINE_PRINTF_SOURCE_DIR "${printf_library_SOURCE_DIR}" CACHE INTERNAL "printf source directory used by Corgo Engine")
set(CE_ENGINE_PRINTF_BINARY_DIR "${printf_library_BINARY_DIR}" CACHE INTERNAL "printf binary directory used by Corgo Engine")

# The Playdate device target gets Cortex-M flags from playdate_game.cmake,
# but third-party targets do not automatically inherit them.
# Ensure printf is built as Thumb code for Cortex-M to avoid UNDEFINSTR faults.
if (TOOLCHAIN STREQUAL "armgcc")
	target_compile_options(printf PRIVATE
		-mthumb
		-mcpu=cortex-m7
		-mfloat-abi=hard
		-mfpu=fpv5-sp-d16
	)
	target_compile_definitions(printf PRIVATE __FPU_USED=1)
endif()

FetchContent_Declare(
	cc
	GIT_REPOSITORY https://github.com/JacksonAllan/CC.git
	GIT_TAG v1.4.3
	SOURCE_DIR ${CE_ENGINE_ROOT_DIR}/3rdParty/cc
)
FetchContent_MakeAvailable(cc)
set(CE_ENGINE_CC_SOURCE_DIR "${cc_SOURCE_DIR}" CACHE INTERNAL "CC include directory used by Corgo Engine")

if (CE_ENGINE_IS_TOP_LEVEL)
	FetchContent_Declare(
		unity
		GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
		GIT_TAG v2.6.1
		SOURCE_DIR ${CE_ENGINE_ROOT_DIR}/3rdParty/unity
	)
	FetchContent_MakeAvailable(unity)
endif()

set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# For single-config generators (NMake/Ninja), default to Debug when unset.
if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Core Files
file(GLOB CE_CORE "${CE_ENGINE_ROOT_DIR}/src/ecs/*.c" "${CE_ENGINE_ROOT_DIR}/src/ecs/*.h" "${CE_ENGINE_ROOT_DIR}/src/ecs/**/*.c" "${CE_ENGINE_ROOT_DIR}/src/ecs/**/*.h" "${CE_ENGINE_ROOT_DIR}/src/ecs/**/**/*.c" "${CE_ENGINE_ROOT_DIR}/src/ecs/**/**/*.h")

# Utils Files
file(GLOB CE_UTILS "${CE_ENGINE_ROOT_DIR}/src/utils/*.c" "${CE_ENGINE_ROOT_DIR}/src/utils/*.h")

# Engine Files
file(GLOB CE_ENGINE "${CE_ENGINE_ROOT_DIR}/src/engine/*.c" "${CE_ENGINE_ROOT_DIR}/src/engine/*.h" "${CE_ENGINE_ROOT_DIR}/src/engine/**/*.c" "${CE_ENGINE_ROOT_DIR}/src/engine/**/*.h")

set(CE_ENGINE_CORE_SOURCES "${CE_CORE}" CACHE INTERNAL "Corgo Engine ECS sources")
set(CE_ENGINE_UTIL_SOURCES "${CE_UTILS}" CACHE INTERNAL "Corgo Engine utility sources")
set(CE_ENGINE_RUNTIME_SOURCES "${CE_ENGINE}" CACHE INTERNAL "Corgo Engine runtime sources")

# global compile definitions
add_compile_definitions(CC_NO_SHORT_NAMES)

# Debug define for both single-config and multi-config generators.
# For Visual Studio, CMAKE_BUILD_TYPE is empty, so use CONFIG generator expression.
add_compile_definitions($<$<CONFIG:Debug>:CE_DEBUG_BUILD>)

# enable warnings as errors
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Generate build info header at build time
set(BUILD_INFO_TEMPLATE "${CE_ENGINE_ROOT_DIR}/src/build_info.h.in")
set(BUILD_INFO_HEADER "${CMAKE_BINARY_DIR}/build_info.h")
set(BUILD_INFO_TIMESTAMP "${CMAKE_BINARY_DIR}/build_info.timestamp")

add_custom_target(ce_generate_build_info_always
	COMMAND ${CMAKE_COMMAND}
		-DINPUT_FILE=${BUILD_INFO_TEMPLATE}
		-DOUTPUT_FILE=${BUILD_INFO_HEADER}
		-P ${CE_ENGINE_ROOT_DIR}/generate_build_info.cmake
	COMMAND ${CMAKE_COMMAND} -E touch ${BUILD_INFO_TIMESTAMP}
	BYPRODUCTS ${BUILD_INFO_HEADER}
	COMMENT "Generating build timestamp"
	VERBATIM
)

add_custom_target(ce_generate_build_info ALL DEPENDS ce_generate_build_info_always)

function(ce_add_playdate_game)
	set(options)
	set(oneValueArgs NAME GAME_DIR GAME_SOURCE_DIR GAME_INCLUDE_ROOT PRODUCT_DIR)
	cmake_parse_arguments(CE_GAME "${options}" "${oneValueArgs}" "" ${ARGN})

	if (NOT CE_GAME_NAME)
		message(FATAL_ERROR "ce_add_playdate_game requires NAME")
	endif()

	if (NOT CE_GAME_GAME_DIR)
		set(CE_GAME_GAME_DIR "${CE_GAME_NAME}")
	endif()

	if (NOT CE_GAME_GAME_SOURCE_DIR)
		set(CE_GAME_GAME_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/${CE_GAME_GAME_DIR}")
	endif()

	if (NOT CE_GAME_GAME_INCLUDE_ROOT)
		set(CE_GAME_GAME_INCLUDE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src")
	endif()

	if (NOT CE_GAME_PRODUCT_DIR)
		set(CE_GAME_PRODUCT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
	endif()

	if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Source/pdxinfo")
		message(FATAL_ERROR
			"ce_add_playdate_game expects a Playdate Source directory at ${CMAKE_CURRENT_SOURCE_DIR}/Source with pdxinfo"
		)
	endif()

	if (NOT EXISTS "${CE_GAME_GAME_SOURCE_DIR}")
		message(FATAL_ERROR "Game source directory not found: ${CE_GAME_GAME_SOURCE_DIR}")
	endif()

	if (CMAKE_BUILD_TYPE STREQUAL "Release")
		set(ce_game_device_name "${CE_GAME_NAME}_RELEASE")
	else()
		set(ce_game_device_name "${CE_GAME_NAME}_DEVICE")
	endif()

	set(ce_game_start_scene_define)
	if (CE_ENGINE_START_SCENE)
		list(APPEND ce_game_start_scene_define CE_ENGINE_SET_START_SCENE=${CE_ENGINE_START_SCENE})
	endif()

	file(GLOB ce_game_sources
		"${CE_GAME_GAME_SOURCE_DIR}/*.c"
		"${CE_GAME_GAME_SOURCE_DIR}/*.h"
		"${CE_GAME_GAME_SOURCE_DIR}/**/*.c"
		"${CE_GAME_GAME_SOURCE_DIR}/**/*.h"
	)

	source_group(TREE "${CE_ENGINE_ROOT_DIR}/src" PREFIX "Engine Files" FILES ${CE_ENGINE_CORE_SOURCES} ${CE_ENGINE_UTIL_SOURCES} ${CE_ENGINE_RUNTIME_SOURCES})
	source_group(TREE "${CE_GAME_GAME_INCLUDE_ROOT}" PREFIX "Game Files" FILES ${ce_game_sources})

	set(PLAYDATE_GAME_NAME "${CE_GAME_NAME}")
	set(PLAYDATE_GAME_DEVICE "${ce_game_device_name}")
	set(SDK "${CE_ENGINE_PLAYDATE_SDK}")
	set(PRODUCT_DIR "${CE_GAME_PRODUCT_DIR}" CACHE STRING "Built PDX directory" FORCE)

	if (TOOLCHAIN STREQUAL "armgcc")
		add_executable(${PLAYDATE_GAME_DEVICE}
			${CE_ENGINE_ROOT_DIR}/src/main.c
			${CE_ENGINE_CORE_SOURCES}
			${CE_ENGINE_UTIL_SOURCES}
			${CE_ENGINE_RUNTIME_SOURCES}
			${ce_game_sources}
		)
		set_target_properties(${PLAYDATE_GAME_DEVICE} PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED YES)
		target_include_directories(${PLAYDATE_GAME_DEVICE} PRIVATE
			${CE_ENGINE_CC_SOURCE_DIR}
			${CE_ENGINE_PRINTF_SOURCE_DIR}/src/printf
			${CE_ENGINE_PRINTF_BINARY_DIR}/include
			${CE_ENGINE_ROOT_DIR}/src
			${CE_GAME_GAME_INCLUDE_ROOT}
			${CMAKE_BINARY_DIR}
		)
		target_compile_definitions(${PLAYDATE_GAME_DEVICE} PRIVATE
			CC_NO_SHORT_NAMES
			$<$<CONFIG:Debug>:CE_DEBUG_BUILD>
			CE_BACKEND_PLAYDATE
			CE_ARM_BUILD
			PRINTF_INCLUDE_CONFIG_H=1
			CE_ENGINE_GAME_DIR=${CE_GAME_GAME_DIR}
			${ce_game_start_scene_define}
		)
		target_link_libraries(${PLAYDATE_GAME_DEVICE} PRIVATE printf)
		add_dependencies(${PLAYDATE_GAME_DEVICE} ce_generate_build_info)
	else()
		add_library(${PLAYDATE_GAME_NAME} SHARED
			${CE_ENGINE_ROOT_DIR}/src/main.c
			${CE_ENGINE_CORE_SOURCES}
			${CE_ENGINE_UTIL_SOURCES}
			${CE_ENGINE_RUNTIME_SOURCES}
			${ce_game_sources}
		)
		set_target_properties(${PLAYDATE_GAME_NAME} PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED YES)
		target_include_directories(${PLAYDATE_GAME_NAME} PRIVATE
			${CE_ENGINE_CC_SOURCE_DIR}
			${CE_ENGINE_PRINTF_SOURCE_DIR}/src/printf
			${CE_ENGINE_ROOT_DIR}/src
			${CE_GAME_GAME_INCLUDE_ROOT}
			${CMAKE_BINARY_DIR}
		)
		target_compile_definitions(${PLAYDATE_GAME_NAME} PRIVATE
			CC_NO_SHORT_NAMES
			$<$<CONFIG:Debug>:CE_DEBUG_BUILD>
			CE_BACKEND_PLAYDATE
			CE_ENGINE_GAME_DIR=${CE_GAME_GAME_DIR}
			${ce_game_start_scene_define}
		)
		target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE printf)
		add_dependencies(${PLAYDATE_GAME_NAME} ce_generate_build_info)
	endif()

	include(${CE_ENGINE_PLAYDATE_SDK}/C_API/buildsupport/playdate_game.cmake)
endfunction()

# Test executable (only build for simulator/host, not device)
if (CE_ENGINE_IS_TOP_LEVEL AND NOT TOOLCHAIN STREQUAL "armgcc")
	enable_testing()

	# Unity test framework library
	add_library(unity_lib STATIC ${unity_SOURCE_DIR}/src/unity.c)
	target_include_directories(unity_lib PUBLIC ${unity_SOURCE_DIR}/src)

	# Disable warnings for Unity (third-party code)
	if(MSVC)
		target_compile_options(unity_lib PRIVATE /wd5045)
	endif()

	# Core tests executable
	add_executable(coretests
		${CE_ENGINE_ROOT_DIR}/src/tests/core.c
		${CE_CORE}
		${CE_UTILS}
		${CE_ENGINE}
	)

	source_group(TREE "${CE_ENGINE_ROOT_DIR}/src" PREFIX "Source Files" FILES ${CE_CORE} ${CE_ENGINE} ${CE_UTILS} ${CE_ENGINE_ROOT_DIR}/src/tests/core.c)

	target_include_directories(coretests PRIVATE
		${cc_SOURCE_DIR}
		${unity_SOURCE_DIR}/src
		${printf_library_SOURCE_DIR}/src/printf
		${CE_ENGINE_ROOT_DIR}/src
		${CMAKE_BINARY_DIR}
	)
	target_link_libraries(coretests PRIVATE unity_lib)
	add_dependencies(coretests ce_generate_build_info)

	# Disable warnings for test executable
	if(MSVC)
		target_compile_options(coretests PRIVATE /wd5045)
	endif()

	# Note: coretests always uses CE_ENGINE_GAME_DIR=game regardless of CE_GAME_NAME
	# to prevent game-specific code from breaking the test suite.
	target_compile_definitions(coretests PRIVATE CE_CORE_TEST_MODE CE_ENGINE_GAME_DIR=game)

	# Register test with CTest
	add_test(NAME CoreTests COMMAND coretests)
endif()

if (CE_ENGINE_IS_TOP_LEVEL)
	ce_add_playdate_game(
		NAME ${CE_GAME_NAME}
		GAME_DIR ${CE_GAME_NAME}
		GAME_SOURCE_DIR "${CE_ENGINE_ROOT_DIR}/src/${CE_GAME_NAME}"
		GAME_INCLUDE_ROOT "${CE_ENGINE_ROOT_DIR}/src"
		PRODUCT_DIR "${CE_ENGINE_ROOT_DIR}"
	)

	# Convenience target to launch the Playdate Simulator (simulator builds only)
	if (NOT TOOLCHAIN STREQUAL "armgcc")
		add_custom_target(run
			COMMAND "${SDK}/bin/PlaydateSimulator.exe" "${CE_ENGINE_ROOT_DIR}/${CE_GAME_NAME}.pdx"
			DEPENDS ${CE_GAME_NAME}
			WORKING_DIRECTORY ${CE_ENGINE_ROOT_DIR}
			COMMENT "Launching Playdate Simulator with ${CE_GAME_NAME}.pdx"
			VERBATIM
		)
	endif()
endif()
