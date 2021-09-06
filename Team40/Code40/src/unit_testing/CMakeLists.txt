# Only keep one main.cpp file or else there will be linker conflicts.
file(GLOB main "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Add path to your unit test files here.
file(GLOB common_models "${CMAKE_CURRENT_SOURCE_DIR}/src/common/models/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/common/models/term/*.cpp")
file(GLOB pkb_srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/*.cpp")

# Extend the executable with the path to your unit test files.
add_executable(unit_testing ${main} ${common_models} ${pkb_srcs})

# Add the relative path to your unit test files to be included in the build.
add_subdirectory(src/pkb)
add_subdirectory(src/common/models)
add_subdirectory(src/common/models/term)

# Ensures that unit_testing runs with spa library.
target_link_libraries(unit_testing spa)

