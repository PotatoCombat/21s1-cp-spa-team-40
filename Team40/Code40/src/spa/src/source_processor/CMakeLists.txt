file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        )

add_library(source_processor ${srcs} include/source_processor/design-extractor/EntityContext.h src/design-extractor/FollowsExtractor.cpp include/source_processor/design-extractor/FollowsExtractor.h)

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(source_processor PUBLIC include)

target_link_libraries(source_processor
        common
        pkb
        )