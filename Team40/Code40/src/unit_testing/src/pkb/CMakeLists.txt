# Only keep one main.cpp file or else there will be linker conflicts.
file(GLOB main "${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp")

# Automatically find your unit test files.
file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# Automatically create pkb_testing, which contains only pkb unit tests.
add_executable(pkb_testing ${main} ${srcs})

# Ensures that unit_testing runs with spa library.
target_link_libraries(pkb_testing spa)

