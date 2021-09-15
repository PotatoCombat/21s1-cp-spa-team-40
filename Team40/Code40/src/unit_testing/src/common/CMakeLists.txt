file(GLOB catch_main "${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp")
file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/models/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/models/conditional/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/models/expression/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/models/relation/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/models/term/*.cpp"
        )

target_sources(unit_testing PRIVATE ${srcs})

# Automatically create pkb_testing, which contains only pkb unit tests.
add_executable(common_testing ${catch_main} ${srcs})

# Ensures that unit_testing runs with spa library.
target_link_libraries(common_testing common)
