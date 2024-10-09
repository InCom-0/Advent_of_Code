add_library(incom_commons ${CMAKE_SOURCE_DIR}/src/internal/incom_commons/incom_commons.h ${CMAKE_SOURCE_DIR}/src/internal/incom_commons/incom_commons.cpp)
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
    std::mdspan)

add_library(incom_omni ${CMAKE_SOURCE_DIR}/src/internal/omni/omni.h)
target_include_directories(incom_omni PUBLIC ${CMAKE_SOURCE_DIR}/src/internal/omni)

target_link_libraries(incom_omni PUBLIC incom_commons)