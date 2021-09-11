# Only keep one main.cpp file or else there will be linker conflicts.
file(GLOB catch_main "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Extend the executable with the path to your unit test files.
add_executable(integration_testing ${catch_main} ${srcs})

# Add the relative path to your unit test files to be included in the build.
#add_subdirectory(src/common)
#add_subdirectory(src/pkb)
#add_subdirectory(src/query_processor)
#add_subdirectory(src/source_processor)

target_include_directories(integration_testing PUBLIC include)

# Ensures that unit_testing runs with spa library.
target_link_libraries(integration_testing spa)
