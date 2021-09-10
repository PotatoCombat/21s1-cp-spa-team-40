file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        )

add_library(pkb ${srcs})

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(pkb PUBLIC include)

#target_link_libraries(pkb spa)
target_link_libraries(pkb common)