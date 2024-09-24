cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ELangCompiler)

include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(project_warnings INTERFACE)
add_library(project_options INTERFACE)

option(project_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
option(project_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)

function(
		set_project_warnings
		project_name
		WARNINGS_AS_ERRORS
		MSVC_WARNINGS
		CLANG_WARNINGS
		GCC_WARNINGS
		CUDA_WARNINGS)
	if("${MSVC_WARNINGS}" STREQUAL "")
		set(MSVC_WARNINGS
			/W4 # Baseline reasonable warnings
			/w14242 # 'identifier': conversion from 'type1' to 'type2', possible loss of data
			/w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
			/w14263 # 'function': member function does not override any base class virtual member function
			/w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not
			# be destructed correctly
			/w14287 # 'operator': unsigned/negative constant mismatch
			/we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside
			# the for-loop scope
			/w14296 # 'operator': expression is always 'boolean_value'
			/w14311 # 'variable': pointer truncation from 'type1' to 'type2'
			/w14545 # expression before comma evaluates to a function which is missing an argument list
			/w14546 # function call before comma missing argument list
			/w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
			/w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
			/w14555 # expression has no effect; expected expression with side- effect
			/w14619 # pragma warning: there is no warning number 'number'
			/w14640 # Enable warning on thread un-safe static member initialization
			/w14826 # Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.
			/w14905 # wide string literal cast to 'LPSTR'
			/w14906 # string literal cast to 'LPWSTR'
			/w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
			/permissive- # standards conformance mode for MSVC compiler.
		)
	endif()

	if("${CLANG_WARNINGS}" STREQUAL "")
		set(CLANG_WARNINGS
			-Wall
			-Wextra # reasonable and standard
			-Wshadow # warn the user if a variable declaration shadows one from a parent context
			-Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps
			# catch hard to track down memory errors
			-Wold-style-cast # warn for c-style casts
			-Wcast-align # warn for potential performance problem casts
			-Wunused # warn on anything being unused
			-Woverloaded-virtual # warn if you overload (not override) a virtual function
			-Wpedantic # warn if non-standard C++ is used
			-Wconversion # warn on type conversions that may lose data
			-Wsign-conversion # warn on sign conversions
			-Wnull-dereference # warn if a null dereference is detected
			-Wdouble-promotion # warn if float is implicit promoted to double
			-Wformat=2 # warn on security issues around functions that format output (ie printf)
			-Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
		)
	endif()

	if("${GCC_WARNINGS}" STREQUAL "")
		set(GCC_WARNINGS
			${CLANG_WARNINGS}
			-Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
			-Wduplicated-cond # warn if if / else chain has duplicated conditions
			-Wduplicated-branches # warn if if / else branches have duplicated code
			-Wlogical-op # warn about logical operations being used where bitwise were probably wanted
			-Wuseless-cast # warn if you perform a cast to the same type
		)
	endif()

	if("${CUDA_WARNINGS}" STREQUAL "")
		set(CUDA_WARNINGS
			-Wall
			-Wextra
			-Wunused
			-Wconversion
			-Wshadow
		)
	endif()

	if(WARNINGS_AS_ERRORS)
		message(STATUS "WARNING TREATED AS ERRORS ENABLED")
		list(APPEND CLANG_WARNINGS -Werror)
		list(APPEND GCC_WARNINGS -Werror)
		list(APPEND MSVC_WARNINGS /WX)
	endif()

	if(MSVC)
		set(PROJECT_WARNINGS_CXX ${MSVC_WARNINGS})
	elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
		set(PROJECT_WARNINGS_CXX ${CLANG_WARNINGS})
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(PROJECT_WARNINGS_CXX ${GCC_WARNINGS})
	else()
		message(WARNING "No compiler warnings set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
	endif()

	# use the same warning flags for C
	set(PROJECT_WARNINGS_C "${PROJECT_WARNINGS_CXX}")

	set(PROJECT_WARNINGS_CUDA "${CUDA_WARNINGS}")

	target_compile_options(
		${project_name}
		INTERFACE # C++ warnings
		$<$<COMPILE_LANGUAGE:CXX>:${PROJECT_WARNINGS_CXX}>
		# C warnings
		$<$<COMPILE_LANGUAGE:C>:${PROJECT_WARNINGS_C}>
		# Cuda warnings
		$<$<COMPILE_LANGUAGE:CUDA>:${PROJECT_WARNINGS_CUDA}>)
endfunction()

set_project_warnings(
	project_warnings
	${project_WARNINGS_AS_ERRORS}
	""
	""
	""
	""
)

macro(project_enable_clang_tidy target WARNINGS_AS_ERRORS)
	find_program(CLANGTIDY clang-tidy)
	if(CLANGTIDY)
		set(CLANG_TIDY_OPTIONS
			${CLANGTIDY}
			-extra-arg=-Wno-unknown-warning-option
			-extra-arg=-Wno-ignored-optimization-argument
			-extra-arg=-Wno-unused-command-line-argument
			-p)

		if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
			if("${CLANG_TIDY_OPTIONS_DRIVER_MODE}" STREQUAL "cl")
				set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=/std:c++${CMAKE_CXX_STANDARD})
			else()
				set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=-std=c++${CMAKE_CXX_STANDARD})
			endif()
		endif()

		if(${WARNINGS_AS_ERRORS})
			list(APPEND CLANG_TIDY_OPTIONS -warnings-as-errors=*)
		endif()

		set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS})
		message(${WARNING_MESSAGE} "clang-tidy set up")
	else()
		message(${WARNING_MESSAGE} "clang-tidy requested but executable not found")
	endif()
endmacro()

project_enable_clang_tidy(project_options ${project_WARNINGS_AS_ERRORS})

target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

include_directories(src)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(__DEBUG__)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(__DIST__)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	add_compile_definitions(__RELEASE__)
endif()

if(${USE_COMPILER} STREQUAL "cl")
	add_compile_definitions(__COMPILER_CL__)
elseif(${USE_COMPILER} STREQUAL "clang-cl")
	add_compile_definitions(__COMPILER_CLANG_CL__)
elseif(${USE_COMPILER} STREQUAL "gcc")
	add_compile_definitions(__COMPILER_GCC__)
elseif(${USE_COMPILER} STREQUAL "clang")
	add_compile_definitions(__COMPILER_CLANG__)
endif()

add_subdirectory(src)
