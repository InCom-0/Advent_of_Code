set(AOC_2015_SRC
    AOC_2015_day_1.cpp
    AOC_2015_day_10.cpp
    AOC_2015_day_11.cpp
    AOC_2015_day_12.cpp
    AOC_2015_day_13.cpp
    AOC_2015_day_14.cpp
    AOC_2015_day_15.cpp
    AOC_2015_day_16.cpp
    AOC_2015_day_17.cpp
    AOC_2015_day_18.cpp
    AOC_2015_day_19.cpp
    AOC_2015_day_2.cpp
    AOC_2015_day_20.cpp
    AOC_2015_day_21.cpp
    AOC_2015_day_22.cpp
    AOC_2015_day_23.cpp
    AOC_2015_day_24.cpp
    AOC_2015_day_25.cpp
    AOC_2015_day_3.cpp
    AOC_2015_day_4.cpp
    AOC_2015_day_5.cpp
    AOC_2015_day_6.cpp
    AOC_2015_day_7.cpp
    AOC_2015_day_8.cpp
    AOC_2015_day_9.cpp
)

list(TRANSFORM AOC_2015_SRC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2015/)
add_library(AOC_2015 ${AOC_2015_SRC})
target_include_directories(AOC_2015 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2015)
target_compile_features(AOC_2015 PRIVATE cxx_std_23)

set(AOC_2016_SRC
    AOC_2016_day_1.cpp
    AOC_2016_day_10.cpp
    AOC_2016_day_11.cpp
    AOC_2016_day_12.cpp
    AOC_2016_day_13.cpp
    AOC_2016_day_14.cpp
    AOC_2016_day_15.cpp
    AOC_2016_day_16.cpp
    AOC_2016_day_17.cpp
    AOC_2016_day_18.cpp
    AOC_2016_day_19.cpp
    AOC_2016_day_2.cpp
    AOC_2016_day_20.cpp
    AOC_2016_day_21.cpp
    AOC_2016_day_22.cpp
    AOC_2016_day_23.cpp
    AOC_2016_day_24.cpp
    AOC_2016_day_25.cpp
    AOC_2016_day_3.cpp
    AOC_2016_day_4.cpp
    AOC_2016_day_5.cpp
    AOC_2016_day_6.cpp
    AOC_2016_day_7.cpp
    AOC_2016_day_8.cpp
    AOC_2016_day_9.cpp
)

list(TRANSFORM AOC_2016_SRC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2016/)
add_library(AOC_2016 ${AOC_2016_SRC})
target_include_directories(AOC_2016 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2016)
target_compile_features(AOC_2016 PRIVATE cxx_std_23)

set(AOC_2017_SRC
    AOC_2017_day_1.cpp
    AOC_2017_day_10.cpp
    AOC_2017_day_11.cpp
    AOC_2017_day_12.cpp
    AOC_2017_day_13.cpp
    AOC_2017_day_14.cpp
    AOC_2017_day_15.cpp
    AOC_2017_day_16.cpp
    AOC_2017_day_17.cpp
    AOC_2017_day_18.cpp
    AOC_2017_day_19.cpp
    AOC_2017_day_2.cpp
    AOC_2017_day_20.cpp
    AOC_2017_day_21.cpp
    AOC_2017_day_22.cpp
    AOC_2017_day_23.cpp
    AOC_2017_day_24.cpp
    AOC_2017_day_25.cpp
    AOC_2017_day_3.cpp
    AOC_2017_day_4.cpp
    AOC_2017_day_5.cpp
    AOC_2017_day_6.cpp
    AOC_2017_day_7.cpp
    AOC_2017_day_8.cpp
    AOC_2017_day_9.cpp
)

list(TRANSFORM AOC_2017_SRC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2017/)
add_library(AOC_2017 ${AOC_2017_SRC})
target_include_directories(AOC_2017 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2017)
target_compile_features(AOC_2017 PRIVATE cxx_std_23)

set(AOC_2018_SRC
    AOC_2018_day_1.cpp
    AOC_2018_day_10.cpp
    AOC_2018_day_11.cpp
    AOC_2018_day_12.cpp
    AOC_2018_day_13.cpp
    AOC_2018_day_14.cpp
    AOC_2018_day_15.cpp
    AOC_2018_day_16.cpp
    AOC_2018_day_17.cpp
    AOC_2018_day_18.cpp
    AOC_2018_day_19.cpp
    AOC_2018_day_2.cpp
    AOC_2018_day_20.cpp
    AOC_2018_day_21.cpp
    AOC_2018_day_22.cpp
    AOC_2018_day_23.cpp
    AOC_2018_day_24.cpp
    AOC_2018_day_25.cpp
    AOC_2018_day_3.cpp
    AOC_2018_day_4.cpp
    AOC_2018_day_5.cpp
    AOC_2018_day_6.cpp
    AOC_2018_day_7.cpp
    AOC_2018_day_8.cpp
    AOC_2018_day_9.cpp
)

list(TRANSFORM AOC_2018_SRC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2018/)
add_library(AOC_2018 ${AOC_2018_SRC})
target_include_directories(AOC_2018 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2018)
target_compile_features(AOC_2018 PRIVATE cxx_std_23)

set(AOC_2019_SRC
    AOC_2019_day_1.cpp
    AOC_2019_day_10.cpp
    AOC_2019_day_11.cpp
    AOC_2019_day_12.cpp
    AOC_2019_day_13.cpp
    AOC_2019_day_14.cpp
    AOC_2019_day_15.cpp
    AOC_2019_day_16.cpp
    AOC_2019_day_17.cpp
    AOC_2019_day_18.cpp
    AOC_2019_day_19.cpp
    AOC_2019_day_2.cpp
    AOC_2019_day_20.cpp
    AOC_2019_day_21.cpp
    AOC_2019_day_22.cpp
    AOC_2019_day_23.cpp
    AOC_2019_day_24.cpp
    AOC_2019_day_25.cpp
    AOC_2019_day_3.cpp
    AOC_2019_day_4.cpp
    AOC_2019_day_5.cpp
    AOC_2019_day_6.cpp
    AOC_2019_day_7.cpp
    AOC_2019_day_8.cpp
    AOC_2019_day_9.cpp
)

list(TRANSFORM AOC_2019_SRC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2019/)
add_library(AOC_2019 ${AOC_2019_SRC})
target_include_directories(AOC_2019 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2019)
target_compile_features(AOC_2019 PRIVATE cxx_std_23)

set(AOC_2024_SRC
    AOC_2024_day_1.cpp
    AOC_2024_day_10.cpp
    AOC_2024_day_11.cpp
    AOC_2024_day_12.cpp
    AOC_2024_day_13.cpp
    AOC_2024_day_14.cpp
    AOC_2024_day_15.cpp
    AOC_2024_day_16.cpp
    AOC_2024_day_17.cpp
    AOC_2024_day_18.cpp
    AOC_2024_day_19.cpp
    AOC_2024_day_2.cpp
    AOC_2024_day_20.cpp
    AOC_2024_day_21.cpp
    AOC_2024_day_22.cpp
    AOC_2024_day_23.cpp
    AOC_2024_day_24.cpp
    AOC_2024_day_25.cpp
    AOC_2024_day_3.cpp
    AOC_2024_day_4.cpp
    AOC_2024_day_5.cpp
    AOC_2024_day_6.cpp
    AOC_2024_day_7.cpp
    AOC_2024_day_8.cpp
    AOC_2024_day_9.cpp
)

list(TRANSFORM AOC_2024_SRC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2024/)
add_library(AOC_2024 ${AOC_2024_SRC})
target_include_directories(AOC_2024 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2024)
target_compile_features(AOC_2024 PRIVATE cxx_std_23)

set(AOC_2025_SRC
    AOC_2025_day_1.cpp
    AOC_2025_day_10.cpp
    AOC_2025_day_11.cpp
    AOC_2025_day_12.cpp
    AOC_2025_day_2.cpp
    AOC_2025_day_3.cpp
    AOC_2025_day_4.cpp
    AOC_2025_day_5.cpp
    AOC_2025_day_6.cpp
    AOC_2025_day_7.cpp
    AOC_2025_day_8.cpp
    AOC_2025_day_9.cpp
)

list(TRANSFORM AOC_2025_SRC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2025/)
add_library(AOC_2025 ${AOC_2025_SRC})
target_include_directories(AOC_2025 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2025)
target_compile_features(AOC_2025 PRIVATE cxx_std_23)
