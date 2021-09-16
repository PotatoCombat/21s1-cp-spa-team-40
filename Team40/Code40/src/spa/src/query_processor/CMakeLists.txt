#CMakeLists for query_processor

file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/parser/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/relationship_handler/*.cpp"
        )
        
add_library(query_processor ${srcs})

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(query_processor PUBLIC include)

target_link_libraries(query_processor pkb)
