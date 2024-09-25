
add_subdirectory(external)
add_subdirectory(internal)
add_subdirectory(problems)



target_link_options(incom_commons PUBLIC -static-libgcc -static-libstdc++ "LINKER:-Bstatic,--whole-archive" -lwinpthread "LINKER:-Bdynamic,--no-whole-archive")

target_link_libraries(AOC_2015 PUBLIC incom_commons)
target_link_libraries(AOC_2016 PUBLIC incom_commons)
target_link_libraries(AOC_2017 PUBLIC incom_commons)
target_link_libraries(AOC_2018 PUBLIC incom_omni incom_commons)

set_target_properties(AOC_2015 PROPERTIES UNITY_BUILD ON)
set_target_properties(AOC_2016 PROPERTIES UNITY_BUILD ON)
set_target_properties(AOC_2017 PROPERTIES UNITY_BUILD ON)
set_target_properties(AOC_2018 PROPERTIES UNITY_BUILD OFF)