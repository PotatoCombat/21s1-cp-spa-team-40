#CMakeLists for query_processor


add_subdirectory(model)
add_subdirectory(query_preprocessor)
add_subdirectory(relationship_handler)

file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(query_processor ${srcs} ${headers})

target_link_directories(query_processor PUBLIC model/ query_preprocessor/ relationship_handler/)

target_link_libraries(query_processor query-processor-model query-preprocessor pkb query-relationship-handler)
#also transitively gets the "entity" and "relationship" dependency
