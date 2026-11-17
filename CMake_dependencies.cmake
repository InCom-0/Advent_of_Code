include(cmake/CPM_0.43.2.cmake)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
CPMAddPackage(
	NAME googletest
	VERSION 1.18.0
	URL
		https://github.com/google/googletest/releases/download/v1.18.0/googletest-1.18.0.tar.gz
		URL_HASH
		SHA256=6e3191c1455468b3fc35a417fb565c1c5071aee1b7e7f85e30cf48a98d37d8b5
)

CPMAddPackage("gh:hanickadot/compile-time-regular-expressions@3.11.0")
CPMAddPackage(
	URI
	"gh:stephenberry/glaze"
	NAME glaze
)
CPMAddPackage(
	URI
	"gh:autodiff/autodiff#main"
	OPTIONS
		"AUTODIFF_BUILD_TESTS OFF"
		"AUTODIFF_BUILD_PYTHON OFF"
		"AUTODIFF_BUILD_EXAMPLES OFF"
		"AUTODIFF_BUILD_DOCS OFF"
)

CPMAddPackage("gh:bkille/bitlib@0.3.1")
CPMAddPackage("gh:tcbrindle/flux@0.4.0")
CPMAddPackage(
	URI
	"gh:InCom-0/hash-library#master"
	OPTIONS "hash-library_BUILD_SHARED_LIB OFF"
)
CPMAddPackage("gh:InCom-0/incstd#main")

CPMAddPackage(
	NAME GeometricTools
	VERSION 8.4
	URL
		https://github.com/davideberly/GeometricTools/archive/refs/tags/GTE-version-8.4.tar.gz
		URL_HASH
		SHA256=3995f59a05981c60024e8a22e2680c789c498ec07c2dbcc9155bda82b9ff6503
	SOURCE_SUBDIR GTE
	OPTIONS
		"GTE_USE_DIRECTX OFF"
		"GTE_BUILD_SHARED_LIBS OFF"
		"GTE_BUILD_MATHEMATICS_GPU OFF"
		"GTE_BUILD_APPLICATIONS OFF"
		"GTE_BUILD_SAMPLES OFF"
)