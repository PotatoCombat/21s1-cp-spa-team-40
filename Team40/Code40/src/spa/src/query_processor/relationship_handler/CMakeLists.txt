#CMakeLists for query_processor/model/relationship_handler

file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(query-relationship-handler STATIC ${srcs} ${headers})
target_link_libraries(query-relationship-handler pkb)
target_link_libraries(query-relationship-handler query-processor-relationship)