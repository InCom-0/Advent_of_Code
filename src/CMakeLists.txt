
add_subdirectory(external)
add_subdirectory(internal)
add_subdirectory(problems)

target_link_libraries(AOC_2015 PUBLIC incom_commons)
target_link_libraries(AOC_2016 PUBLIC incom_commons)
target_link_libraries(AOC_2017 PUBLIC incom_commons)
target_link_libraries(AOC_2018 PUBLIC incom_commons incom_omni)
target_link_libraries(AOC_2019 PUBLIC incom_commons incom_omni incom_seq)
target_link_libraries(AOC_2024 PUBLIC incom_commons incom_omni incom_seq)

set_target_properties(AOC_2015 PROPERTIES UNITY_BUILD ON)
set_target_properties(AOC_2016 PROPERTIES UNITY_BUILD ON)
set_target_properties(AOC_2017 PROPERTIES UNITY_BUILD ON)
set_target_properties(AOC_2018 PROPERTIES UNITY_BUILD ON)
set_target_properties(AOC_2019 PROPERTIES UNITY_BUILD OFF)
set_target_properties(AOC_2024 PROPERTIES UNITY_BUILD OFF)