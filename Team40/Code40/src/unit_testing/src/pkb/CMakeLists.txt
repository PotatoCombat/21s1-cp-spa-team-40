file(GLOB catch_main "${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp")
file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../include/TestUtils.h")

target_sources(unit_testing PRIVATE ${srcs})

# Automatically create pkb_testing, which contains only pkb unit tests.
add_executable(pkb_testing ${catch_main} ${srcs})

# Ensures that unit_testing runs with spa library.
target_link_libraries(pkb_testing pkb)
