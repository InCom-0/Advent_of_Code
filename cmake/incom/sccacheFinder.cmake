function(configure_sccache)
    set(_sccache_options REQUIRED)
    cmake_parse_arguments(SCCACHE_FINDER "${_sccache_options}" "" "" ${ARGN})

    if(SCCACHE_FINDER_UNPARSED_ARGUMENTS)
        message(WARNING
            "configure_sccache received unsupported arguments. Only REQUIRED is permitted. Unsupported arguments: [${SCCACHE_FINDER_UNPARSED_ARGUMENTS}]"
        )
    endif()

    find_program(_SCCACHE_EXECUTABLE NAMES sccache)
    if(_SCCACHE_EXECUTABLE)
        set(SCCACHE "${_SCCACHE_EXECUTABLE}" PARENT_SCOPE)
        set(_sccache_launcher ${MSYS2_ENVIRONMENT_LAUNCHER} "${_SCCACHE_EXECUTABLE}")
        set(CMAKE_C_COMPILER_LAUNCHER "${_sccache_launcher}" PARENT_SCOPE)
        set(CMAKE_CXX_COMPILER_LAUNCHER "${_sccache_launcher}" PARENT_SCOPE)
        message(STATUS "sccache found: ${_SCCACHE_EXECUTABLE}")

        ### sccache requires the following settings on MSVC
        if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC")
            set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>")
            cmake_policy(SET CMP0141 NEW)
        endif()

    else()
        if(SCCACHE_FINDER_REQUIRED)
            message(FATAL_ERROR
                "sccache is required but was not found in PATH. Install sccache or call configure_sccache() without REQUIRED."
            )
        else()
            unset(SCCACHE PARENT_SCOPE)
            if(MSYS2_ENVIRONMENT_LAUNCHER)
                set(CMAKE_C_COMPILER_LAUNCHER "${MSYS2_ENVIRONMENT_LAUNCHER}" PARENT_SCOPE)
                set(CMAKE_CXX_COMPILER_LAUNCHER "${MSYS2_ENVIRONMENT_LAUNCHER}" PARENT_SCOPE)
            else()
                unset(CMAKE_C_COMPILER_LAUNCHER PARENT_SCOPE)
                unset(CMAKE_CXX_COMPILER_LAUNCHER PARENT_SCOPE)
            endif()
            message(WARNING
                "sccache was not found in PATH. Continuing without compiler cache."
            )
        endif()
    endif()
endfunction()
