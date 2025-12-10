add_library(incom_commons ${CMAKE_SOURCE_DIR}/src/internal/incom_commons/incom_commons.cpp)
target_include_directories(incom_commons PUBLIC ${CMAKE_SOURCE_DIR}/src/internal/incom_commons)

target_link_libraries(incom_commons PUBLIC
    fmt::fmt
    glaze::glaze
    flux::flux
    ctre::ctre
    xxHash::xxhash
    bitlib::bitlib
    unordered_dense::unordered_dense
    more_concepts::more_concepts
    hash-library::hash-library
    mdspan::mdspan)

# target_link_options(incom_commons PUBLIC -static-libgcc -static-libstdc++ "LINKER:-Bstatic,--whole-archive" -lwinpthread "LINKER:-Bdynamic,--no-whole-archive")

include(../../cmake/stdlibQuery.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        message(STATUS "GCC Compiler")
        message(STATUS "Using libstdc++")
        target_link_options(incom_commons PUBLIC -static-libgcc -static-libstdc++ "LINKER:-Bstatic,--whole-archive" -lwinpthread "LINKER:-Bdynamic,--no-whole-archive")

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
                target_link_options(incom_commons PUBLIC -stdlib=libc++)
            elseif(USING_LIBSTDCXX)
                message(STATUS "Using libstdc++")
                target_link_options(incom_commons PUBLIC -static-libgcc -static-libstdc++)
            endif()
        else()
            target_link_options(incom_commons PUBLIC -static)
            if(USING_LIBCXX)
                message(STATUS "Using libc++")
                target_link_options(incom_commons PUBLIC -stdlib=libc++ -Wl,-Bstatic -lc++ -lc++abi -Wl,-Bdynamic)
            elseif(USING_LIBSTDCXX)
                message(STATUS "Using libstdc++")
                target_link_options(incom_commons PUBLIC -static-libgcc -static-libstdc++)
            endif()
        endif()

    else()
        message(WARNING "You are using an unexpected compiler (ie. neither GCC, Clang, AppleClang, MSVC). All bets are off.")
    endif()
endif()




add_library(incom_omni ${CMAKE_SOURCE_DIR}/src/internal/incom_omni/incom_omni.h ${CMAKE_SOURCE_DIR}/src/internal/incom_omni/incom_omni.cpp)
target_include_directories(incom_omni PUBLIC ${CMAKE_SOURCE_DIR}/src/internal/incom_omni)

target_link_libraries(incom_omni PUBLIC incom_commons)

add_library(incom_seq ${CMAKE_SOURCE_DIR}/src/internal/incom_seq/incom_seq.h ${CMAKE_SOURCE_DIR}/src/internal/incom_seq/incom_seq.cpp)
target_include_directories(incom_seq PUBLIC ${CMAKE_SOURCE_DIR}/src/internal/incom_seq)

target_link_libraries(incom_seq PUBLIC incom_commons)
