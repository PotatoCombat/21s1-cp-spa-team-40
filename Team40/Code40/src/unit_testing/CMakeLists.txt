# Only keep one main.cpp file or else there will be linker conflicts.
file(GLOB catch_main "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Extend the executable with the path to your unit test files.
add_executable(unit_testing ${catch_main})

# Add the relative path to your unit test files to be included in the build.
add_subdirectory(src/common)
add_subdirectory(src/pkb)
add_subdirectory(src/query_processor)

# Ensures that unit_testing runs with spa library.
target_link_libraries(unit_testing spa)
