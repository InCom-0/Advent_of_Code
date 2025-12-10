include(cmake/CPM.cmake)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
CPMAddPackage("gh:google/googletest@1.17.0")

CPMAddPackage("gh:hanickadot/compile-time-regular-expressions@3.10.0")
CPMAddPackage("gh:stephenberry/glaze@6.1.0")

CPMAddPackage(
  NAME     fmtlib
  URL      https://github.com/fmtlib/fmt/releases/download/12.1.0/fmt-12.1.0.zip
  URL_HASH SHA256=695fd197fa5aff8fc67b5f2bbc110490a875cdf7a41686ac8512fb480fa8ada7
  EXCLUDE_FROM_ALL TRUE
)

CPMAddPackage("gh:bkille/bitlib@0.3.1")
CPMAddPackage("gh:tcbrindle/flux@0.4.0")
CPMAddPackage("gh:MiSo1289/more_concepts#master")
CPMAddPackage("gh:InCom-0/hash-library#master")
CPMAddPackage("gh:InCom-0/mdspan#stable")

CPMAddPackage(
    URI "gh:Cyan4973/xxHash@0.8.3"
    SOURCE_SUBDIR cmake_unofficial
)
CPMAddPackage("gh:martinus/unordered_dense@4.8.1")