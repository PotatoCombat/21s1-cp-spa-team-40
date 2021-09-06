file(GLOB main "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
file(GLOB pkb_srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/*.cpp")

add_executable(unit_testing ${main} ${pkb_srcs})

target_link_libraries(unit_testing spa)

add_subdirectory(src/pkb)