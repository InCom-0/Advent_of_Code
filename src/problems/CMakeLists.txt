set(AOC_2015_SRC
    AOC_2015_day_1.cpp
    AOC_2015_day_1.h
    AOC_2015_day_10.cpp
    AOC_2015_day_10.h
    AOC_2015_day_11.cpp
    AOC_2015_day_11.h
    AOC_2015_day_12.cpp
    AOC_2015_day_12.h
    AOC_2015_day_13.cpp
    AOC_2015_day_13.h
    AOC_2015_day_14.cpp
    AOC_2015_day_14.h
    AOC_2015_day_15.cpp
    AOC_2015_day_15.h
    AOC_2015_day_16.cpp
    AOC_2015_day_16.h
    AOC_2015_day_17.cpp
    AOC_2015_day_17.h
    AOC_2015_day_18.cpp
    AOC_2015_day_18.h
    AOC_2015_day_19.cpp
    AOC_2015_day_19.h
    AOC_2015_day_2.cpp
    AOC_2015_day_2.h
    AOC_2015_day_20.cpp
    AOC_2015_day_20.h
    AOC_2015_day_21.cpp
    AOC_2015_day_21.h
    AOC_2015_day_22.cpp
    AOC_2015_day_22.h
    AOC_2015_day_23.cpp
    AOC_2015_day_23.h
    AOC_2015_day_24.cpp
    AOC_2015_day_24.h
    AOC_2015_day_25.cpp
    AOC_2015_day_25.h
    AOC_2015_day_3.cpp
    AOC_2015_day_3.h
    AOC_2015_day_4.cpp
    AOC_2015_day_4.h
    AOC_2015_day_5.cpp
    AOC_2015_day_5.h
    AOC_2015_day_6.cpp
    AOC_2015_day_6.h
    AOC_2015_day_7.cpp
    AOC_2015_day_7.h
    AOC_2015_day_8.cpp
    AOC_2015_day_8.h
    AOC_2015_day_9.cpp
    AOC_2015_day_9.h)

list(TRANSFORM AOC_2015_SRC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2015/)
add_library(AOC_2015 ${AOC_2015_SRC} ${CMAKE_SOURCE_DIR}/include/AOC_2015.h)
target_include_directories(AOC_2015 PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2015)

set(AOC_2016_SRC
    AOC_2016_day_1.cpp
    AOC_2016_day_1.h
    AOC_2016_day_10.cpp
    AOC_2016_day_10.h
    AOC_2016_day_11.cpp
    AOC_2016_day_11.h
    AOC_2016_day_12.cpp
    AOC_2016_day_12.h
    AOC_2016_day_13.cpp
    AOC_2016_day_13.h
    AOC_2016_day_14.cpp
    AOC_2016_day_14.h
    AOC_2016_day_15.cpp
    AOC_2016_day_15.h
    AOC_2016_day_16.cpp
    AOC_2016_day_16.h
    AOC_2016_day_17.cpp
    AOC_2016_day_17.h
    AOC_2016_day_18.cpp
    AOC_2016_day_18.h
    AOC_2016_day_19.cpp
    AOC_2016_day_19.h
    AOC_2016_day_2.cpp
    AOC_2016_day_2.h
    AOC_2016_day_20.cpp
    AOC_2016_day_20.h
    AOC_2016_day_21.cpp
    AOC_2016_day_21.h
    AOC_2016_day_22.cpp
    AOC_2016_day_22.h
    AOC_2016_day_23.cpp
    AOC_2016_day_23.h
    AOC_2016_day_24.cpp
    AOC_2016_day_24.h
    AOC_2016_day_25.cpp
    AOC_2016_day_25.h
    AOC_2016_day_3.cpp
    AOC_2016_day_3.h
    AOC_2016_day_4.cpp
    AOC_2016_day_4.h
    AOC_2016_day_5.cpp
    AOC_2016_day_5.h
    AOC_2016_day_6.cpp
    AOC_2016_day_6.h
    AOC_2016_day_7.cpp
    AOC_2016_day_7.h
    AOC_2016_day_8.cpp
    AOC_2016_day_8.h
    AOC_2016_day_9.cpp
    AOC_2016_day_9.h)

list(TRANSFORM AOC_2016_SRC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2016/)
add_library(AOC_2016 ${AOC_2016_SRC} ${CMAKE_SOURCE_DIR}/include/AOC_2016.h)
target_include_directories(AOC_2016 PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2016)

set(AOC_2017_SRC
    AOC_2017_day_1.cpp
    AOC_2017_day_1.h
    AOC_2017_day_10.cpp
    AOC_2017_day_10.h
    AOC_2017_day_11.cpp
    AOC_2017_day_11.h
    AOC_2017_day_12.cpp
    AOC_2017_day_12.h
    AOC_2017_day_13.cpp
    AOC_2017_day_13.h
    AOC_2017_day_14.cpp
    AOC_2017_day_14.h
    AOC_2017_day_15.cpp
    AOC_2017_day_15.h
    AOC_2017_day_16.cpp
    AOC_2017_day_16.h
    AOC_2017_day_17.cpp
    AOC_2017_day_17.h
    AOC_2017_day_18.cpp
    AOC_2017_day_18.h
    AOC_2017_day_19.cpp
    AOC_2017_day_19.h
    AOC_2017_day_2.cpp
    AOC_2017_day_2.h
    AOC_2017_day_20.cpp
    AOC_2017_day_20.h
    AOC_2017_day_21.cpp
    AOC_2017_day_21.h
    AOC_2017_day_22.cpp
    AOC_2017_day_22.h
    AOC_2017_day_23.cpp
    AOC_2017_day_23.h
    AOC_2017_day_24.cpp
    AOC_2017_day_24.h
    AOC_2017_day_25.cpp
    AOC_2017_day_25.h
    AOC_2017_day_3.cpp
    AOC_2017_day_3.h
    AOC_2017_day_4.cpp
    AOC_2017_day_4.h
    AOC_2017_day_5.cpp
    AOC_2017_day_5.h
    AOC_2017_day_6.cpp
    AOC_2017_day_6.h
    AOC_2017_day_7.cpp
    AOC_2017_day_7.h
    AOC_2017_day_8.cpp
    AOC_2017_day_8.h
    AOC_2017_day_9.cpp
    AOC_2017_day_9.h)

list(TRANSFORM AOC_2017_SRC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2017/)
add_library(AOC_2017 ${AOC_2017_SRC} ${CMAKE_SOURCE_DIR}/include/AOC_2017.h)
target_include_directories(AOC_2017 PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2017)

set(AOC_2018_SRC
    AOC_2018_TEMP.cpp
    AOC_2018_TEMP.h
    AOC_2018_day_1.cpp
    AOC_2018_day_1.h
    AOC_2018_day_10.cpp
    AOC_2018_day_10.h
    AOC_2018_day_11.cpp
    AOC_2018_day_11.h
    AOC_2018_day_12.cpp
    AOC_2018_day_12.h
    AOC_2018_day_13.cpp
    AOC_2018_day_13.h
    AOC_2018_day_14.cpp
    AOC_2018_day_14.h
    AOC_2018_day_15.cpp
    AOC_2018_day_15.h
    AOC_2018_day_16.cpp
    AOC_2018_day_16.h
    AOC_2018_day_17.cpp
    AOC_2018_day_17.h
    AOC_2018_day_18.cpp
    AOC_2018_day_18.h
    AOC_2018_day_19.cpp
    AOC_2018_day_19.h
    AOC_2018_day_2.cpp
    AOC_2018_day_2.h
    AOC_2018_day_20.cpp
    AOC_2018_day_20.h
    AOC_2018_day_21.cpp
    AOC_2018_day_21.h
    AOC_2018_day_22.cpp
    AOC_2018_day_22.h
    AOC_2018_day_23.cpp
    AOC_2018_day_23.h
    AOC_2018_day_24.cpp
    AOC_2018_day_24.h
    AOC_2018_day_25.cpp
    AOC_2018_day_25.h
    AOC_2018_day_3.cpp
    AOC_2018_day_3.h
    AOC_2018_day_4.cpp
    AOC_2018_day_4.h
    AOC_2018_day_5.cpp
    AOC_2018_day_5.h
    AOC_2018_day_6.cpp
    AOC_2018_day_6.h
    AOC_2018_day_7.cpp
    AOC_2018_day_7.h
    AOC_2018_day_8.cpp
    AOC_2018_day_8.h
    AOC_2018_day_9.cpp
    AOC_2018_day_9.h)

list(TRANSFORM AOC_2018_SRC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2018/)
add_library(AOC_2018 ${AOC_2018_SRC} ${CMAKE_SOURCE_DIR}/include/AOC_2018.h)
target_include_directories(AOC_2018 PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2018)


set(AOC_2019_SRC
    AOC_2019_day_1.cpp
    AOC_2019_day_1.h
    AOC_2019_day_10.cpp
    AOC_2019_day_10.h
    AOC_2019_day_11.cpp
    AOC_2019_day_11.h
    AOC_2019_day_12.cpp
    AOC_2019_day_12.h
    AOC_2019_day_13.cpp
    AOC_2019_day_13.h
    AOC_2019_day_14.cpp
    AOC_2019_day_14.h
    AOC_2019_day_15.cpp
    AOC_2019_day_15.h
    AOC_2019_day_16.cpp
    AOC_2019_day_16.h
    AOC_2019_day_17.cpp
    AOC_2019_day_17.h
    AOC_2019_day_18.cpp
    AOC_2019_day_18.h
    AOC_2019_day_19.cpp
    AOC_2019_day_19.h
    AOC_2019_day_2.cpp
    AOC_2019_day_2.h
    AOC_2019_day_20.cpp
    AOC_2019_day_20.h
    AOC_2019_day_21.cpp
    AOC_2019_day_21.h
    AOC_2019_day_22.cpp
    AOC_2019_day_22.h
    AOC_2019_day_23.cpp
    AOC_2019_day_23.h
    AOC_2019_day_24.cpp
    AOC_2019_day_24.h
    AOC_2019_day_25.cpp
    AOC_2019_day_25.h
    AOC_2019_day_3.cpp
    AOC_2019_day_3.h
    AOC_2019_day_4.cpp
    AOC_2019_day_4.h
    AOC_2019_day_5.cpp
    AOC_2019_day_5.h
    AOC_2019_day_6.cpp
    AOC_2019_day_6.h
    AOC_2019_day_7.cpp
    AOC_2019_day_7.h
    AOC_2019_day_8.cpp
    AOC_2019_day_8.h
    AOC_2019_day_9.cpp
    AOC_2019_day_9.h)

list(TRANSFORM AOC_2019_SRC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2019/)
add_library(AOC_2019 ${AOC_2019_SRC} ${CMAKE_SOURCE_DIR}/include/AOC_2019.h)
target_include_directories(AOC_2019 PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/AOC_2019)