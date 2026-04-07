include(cmake/CPM_0.42.1.cmake)


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
CPMAddPackage("gh:google/googletest@1.17.0")

CPMAddPackage("gh:hanickadot/compile-time-regular-expressions@3.10.0")
CPMAddPackage(
    URI "gh:stephenberry/glaze@7.2.2"
    NAME glaze
)

CPMAddPackage(
    URL https://github.com/fmtlib/fmt/releases/download/12.1.0/fmt-12.1.0.zip
    URL_HASH SHA256=695fd197fa5aff8fc67b5f2bbc110490a875cdf7a41686ac8512fb480fa8ada7
    NAME fmt
    EXCLUDE_FROM_ALL TRUE
)

CPMAddPackage("gh:bkille/bitlib@0.3.1")
CPMAddPackage("gh:tcbrindle/flux@0.4.0")
CPMAddPackage(
    URI "gh:InCom-0/hash-library#master"
    OPTIONS
    "hash-library_BUILD_SHARED_LIB OFF"
)
CPMAddPackage("gh:InCom-0/mdspan#stable")

CPMAddPackage("gh:InCom-0/incstd#main")
