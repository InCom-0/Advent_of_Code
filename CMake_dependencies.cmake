include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    bitlib
    GIT_REPOSITORY https://github.com/bkille/bitlib.git
    GIT_TAG origin/master
)
FetchContent_MakeAvailable(bitlib)

FetchContent_Declare(
    flux
    GIT_REPOSITORY https://github.com/tcbrindle/flux.git
    GIT_TAG v0.4.0
)
FetchContent_MakeAvailable(flux)

FetchContent_Declare(
    more_concepts
    GIT_REPOSITORY https://github.com/MiSo1289/more_concepts.git
    GIT_TAG origin/master
)
FetchContent_MakeAvailable(more_concepts)

FetchContent_Declare(
    hash-library
    GIT_REPOSITORY https://github.com/InCom-0/hash-library
    GIT_TAG master
)
FetchContent_MakeAvailable(hash-library)

FetchContent_Declare(
    mdspan
    GIT_REPOSITORY https://github.com/InCom-0/mdspan
    GIT_TAG stable
)
FetchContent_MakeAvailable(mdspan)

FetchContent_Declare(
    functional
    GIT_REPOSITORY https://github.com/InCom-0/functional
    GIT_TAG main
)
FetchContent_MakeAvailable(functional)


# FetchContent_Declare(
#     foonathan_memory
#     GIT_REPOSITORY https://github.com/foonathan/memory.git
#     GIT_TAG main
# )
# FetchContent_MakeAvailable(foonathan_memory)

find_package(fmt CONFIG REQUIRED)
find_package(glaze REQUIRED)
find_package(ctre REQUIRED)
find_package(xxHash REQUIRED)
find_package(unordered_dense REQUIRED)