# This contains the file with CATCH_CONFIG_MAIN
file(GLOB catch_main "${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp")

# You must list subfolders here with *.cpp
file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/design_extractor/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/parser/*.cpp"
        )

# This adds all source files into unit_testing executable
target_sources(unit_testing PRIVATE ${srcs})

# This creates a subset of unit_testing for faster build times (called source_processor_testing)
add_executable(source_processor_testing ${catch_main} ${srcs})

target_include_directories(source_processor_testing PUBLIC include)

# Ensures that source_processor_testing runs with spa library.
target_link_libraries(source_processor_testing source_processor)
