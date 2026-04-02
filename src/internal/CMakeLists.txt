add_library(incom_commons INTERFACE)
target_sources(incom_commons
    INTERFACE
    FILE_SET commons_pub_headers
    TYPE HEADERS
    BASE_DIRS
    incom_commons/
)

target_link_libraries(incom_commons INTERFACE
    fmt::fmt
    glaze::glaze
    flux::flux
    ctre::ctre
    incstd::incstd
    bitlib::bitlib
    hash-library::hash-library
    mdspan::mdspan
)

# target_link_options(incom_commons PUBLIC -static-libgcc -static-libstdc++ "LINKER:-Bstatic,--whole-archive" -lwinpthread "LINKER:-Bdynamic,--no-whole-archive")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        message(STATUS "GCC Compiler")
        message(STATUS "Using libstdc++")
        target_link_options(incom_commons INTERFACE -static)

    elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC")
        if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            message(STATUS "MSVC Compiler")
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            message(STATUS "Clang-cl Compiler")
        endif()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        # set_property(TARGET incom_commons PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
        message(STATUS "Clang Compiler")
        if(APPLE)
            if(USING_LIBCXX)
                message(STATUS "Using libc++")
                target_link_options(incom_commons INTERFACE -stdlib=libc++)
            elseif(USING_LIBSTDCXX)
                message(STATUS "Using libstdc++")
                target_link_options(incom_commons INTERFACE -static)
            endif()
        else()
            target_link_options(incom_commons INTERFACE -static)
            if(USING_LIBCXX)
                message(STATUS "Using libc++")
                target_link_options(incom_commons INTERFACE -stdlib=libc++ -Wl,-Bstatic -lc++ -lc++abi -Wl,-Bdynamic)
            elseif(USING_LIBSTDCXX)
                message(STATUS "Using libstdc++")
                target_link_options(incom_commons INTERFACE -static)
            endif()
        endif()

    else()
        message(WARNING "You are using an unexpected compiler (ie. neither GCC, Clang, AppleClang, MSVC). All bets are off.")
    endif()
endif()


add_library(incom_omni INTERFACE)
target_sources(incom_omni
    INTERFACE
    FILE_SET omni_pub_headers
    TYPE HEADERS
    BASE_DIRS
    incom_omni/
)
target_link_libraries(incom_omni INTERFACE incom_commons)

add_library(incom_seq INTERFACE)
target_sources(incom_seq
    INTERFACE
    FILE_SET seq_pub_headers
    TYPE HEADERS
    BASE_DIRS
    incom_seq/
)
target_link_libraries(incom_seq INTERFACE incom_commons)
