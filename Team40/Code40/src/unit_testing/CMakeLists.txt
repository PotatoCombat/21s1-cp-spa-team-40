# Only keep one main.cpp file or else there will be linker conflicts.
file(GLOB main "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Add path to your unit test files here.
file(GLOB pkb_srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/*.cpp")

# Extend the executable with the path to your unit test files.
add_executable(unit_testing ${main} ${pkb_srcs})

# Add the relative path to your unit test files to be included in the build.
add_subdirectory(src/pkb)

# Ensures that unit_testing runs with spa library.
target_link_libraries(unit_testing spa)

