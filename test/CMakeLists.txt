enable_testing()

add_executable(AOC_Tests_2015)
target_sources(AOC_Tests_2015 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2015_test.cpp)

add_executable(AOC_Tests_2016)
target_sources(AOC_Tests_2016 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2016_test.cpp)

add_executable(AOC_Tests_2017)
target_sources(AOC_Tests_2017 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2017_test.cpp)

add_executable(AOC_Tests_2018)
target_sources(AOC_Tests_2018 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2018_test.cpp)

add_executable(AOC_Tests_2019)
target_sources(AOC_Tests_2019 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2019_test.cpp)

add_executable(AOC_Tests_ALL)
target_sources(AOC_Tests_ALL PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2015_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2016_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2017_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2018_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2019_test.cpp
    )

target_link_libraries(AOC_Tests_2015 PRIVATE AOC_2015 gtest_main gmock_main)
target_link_libraries(AOC_Tests_2016 PRIVATE AOC_2016 gtest_main gmock_main)
target_link_libraries(AOC_Tests_2017 PRIVATE AOC_2017 gtest_main gmock_main)
target_link_libraries(AOC_Tests_2018 PRIVATE AOC_2018 gtest_main gmock_main)
target_link_libraries(AOC_Tests_2019 PRIVATE AOC_2019 gtest_main gmock_main)
target_link_libraries(AOC_Tests_ALL PRIVATE AOC_2015 AOC_2016 AOC_2017 AOC_2018 AOC_2019 gtest_main gmock_main)